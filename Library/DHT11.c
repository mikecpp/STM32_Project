#include "DHT11.h"
#include "timer.h"

#define MAX_RETRIES                  100

#define DEBUG   0

uint8_t m_dht_type = DHT_11;

void DHT11_IO_IN()  
{
    GPIO_InitTypeDef GPIO_Initure;
   
    GPIO_Initure.Pin    = DHT11_GPIO_PIN;         
    GPIO_Initure.Mode   = GPIO_MODE_INPUT;   
    HAL_GPIO_Init(DHT11_GPIO_PORT, &GPIO_Initure);    
}

void DHT11_IO_OUT() 
{ 
    GPIO_InitTypeDef GPIO_Initure;
   
    GPIO_Initure.Pin    = DHT11_GPIO_PIN;         
    GPIO_Initure.Mode   = GPIO_MODE_OUTPUT_PP;   
    GPIO_Initure.Speed  = GPIO_SPEED_HIGH;        
    HAL_GPIO_Init(DHT11_GPIO_PORT, &GPIO_Initure);      
}

void DHT11_DQ_OUT(uint8_t enable) 
{ 
    if(enable) {
        HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);
    }
    else {
        HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET);
    }
}    
        
uint8_t DHT11_DQ_IN()
{
    return HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
}

void DHT11_reset(void)
{
	DHT11_IO_OUT(); 	
    
    DHT11_DQ_OUT(0); 	    // LOW
    delay_ms(20);    	
    DHT11_DQ_OUT(1); 	    // HIGH
  
	timer_delay_us(30);     	
	DHT11_IO_IN();   
}

uint8_t DHT11_Check(void)
{
	uint8_t retry = 0;
    
    while(DHT11_DQ_IN() && retry < MAX_RETRIES)       // HIGH
	{
		retry++;
		timer_delay_us(1);
	}
    
	if(retry >= MAX_RETRIES) {
        return 1;
    }
	else {
        retry = 0;
    }
    
    while(!DHT11_DQ_IN() && retry < MAX_RETRIES)      // LOW
	{
		retry++;
		timer_delay_us(1);
	}
    
	if(retry>=100) {
        return 1;	
    }        

	return 0;    
}

uint8_t DHT11_Read_Bit(void)
{
    uint8_t retry = 0;

	while(DHT11_DQ_IN() && retry < MAX_RETRIES)  // HIGH --> LOW
	{
		retry++;
		timer_delay_us(1);
	}
      
	while(!DHT11_DQ_IN() && retry < MAX_RETRIES) // LOW --> HIGH 
	{
		retry++;
		timer_delay_us(1);
	}   
    
	timer_delay_us(40);
    
    return DHT11_DQ_IN();	
}

uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, dat = 0;
    
	for(i=0; i<8; i++) 
	{
   		dat <<= 1; 
	    dat |= DHT11_Read_Bit();
    }			
    
    return dat;    
}

//////////////////////////////////////////////////////////////////////////////
// DHT11 Public API
//
int DHT11_init(DHT_TYPE type)
{
    GPIO_InitTypeDef GPIO_Initure;

    timer_init();
    
    DHT11_GPIO_CLK_ENABLE();
	
    GPIO_Initure.Pin    = DHT11_GPIO_PIN;         
    GPIO_Initure.Mode   = GPIO_MODE_OUTPUT_PP;   
    GPIO_Initure.Pull   = GPIO_PULLUP;           
    GPIO_Initure.Speed  = GPIO_SPEED_HIGH;     
    HAL_GPIO_Init(DHT11_GPIO_PORT, &GPIO_Initure);     
 
    DHT11_reset();

    m_dht_type = type;
    
	return DHT11_Check();    
}

int DHT11_read(uint16_t *temp, uint16_t *humi)
{
 	uint8_t buf[5];
	uint8_t i;
    
	DHT11_reset();
    
	if(DHT11_Check() != 0) {
        return 1;
    }
    
    for(i=0; i<5; i++) {
        buf[i] = DHT11_Read_Byte();
	}

#if DEBUG    
    for(i=0; i<5; i++) {
        printf("%d: %02X \r\n", i, buf[i]);
    }    
#endif    
 
    uint8_t chksum = buf[0]+buf[1]+buf[2]+buf[3];
    if(chksum != buf[4]) {
        printf("Check-sum = %02X, Buf-04 = %02X \n", chksum, buf[4]);
        return -1;
    }    
    
    if(m_dht_type == DHT_11) {
        *humi = buf[0]; 
        *temp = buf[2]; 
    }
    else {
        *humi = (buf[0] << 8) | buf[1];
        *temp = (buf[2] << 8) | buf[3];
    }
    
    return 0;
}    
