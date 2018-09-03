#include "stm32f469_system.h" 
#include "uart.h"
#include "stm32469i_discovery_lcd.h"
#include <stdlib.h>
#include "cmsis_os.h"

#define X_SIZE      800
#define Y_SIZE      480

static void fillrect_test(void *param)
{   
    uint32_t x, y;
    uint32_t c;
    
    BSP_LCD_Clear(LCD_COLOR_BLACK);   

    while(1) {
        x = rand() % 300;
        y = rand() % Y_SIZE;
        c = 0xFF000000 + (rand() % 0x00FFFFFF);
		BSP_LCD_SetTextColor(c);
        BSP_LCD_FillRect(x, y, 100, 100);
        vTaskDelay(10);
    }
}

static void rectangle_test(void *param)
{
    uint32_t x, y;
    uint32_t c;
    
    BSP_LCD_Clear(LCD_COLOR_BLACK);   

    while(1) {
        x = 400 + (rand() % 400);
        if(x >= 700) 
            x = 700;
        y = rand() % Y_SIZE;
        c = 0xFF000000 + (rand() % 0x00FFFFFF);
        // printf("x = %d, y = %d, c = %X\r\n", x, y, c);
		BSP_LCD_SetTextColor(c);
        BSP_LCD_DrawRect(x, y, 100, 100);
        vTaskDelay(10);
    }		
}

int main(void)
{
    system_init();
    
    uart_init(UART_3, 115200);
    uart_consol(UART_3);
    
    printf("Hello World !\r\n");
 
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);       
    BSP_LCD_Clear(LCD_COLOR_BLACK); 

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);      
    BSP_LCD_DisplayStringAt(0, 0,  (uint8_t*) "NT35510 Demo", CENTER_MODE);

    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);  
    BSP_LCD_DisplayStringAtLine(1, (uint8_t*) "Hello Mike");
    BSP_LCD_DisplayStringAtLine(2, (uint8_t*) "ABCDE1234567890");    
    
    delay_ms(1000);
    
    xTaskCreate(fillrect_test, "Fill Test", 1000, NULL, 5, NULL); 
    xTaskCreate(rectangle_test, "Rect Test", 1000, NULL, 6, NULL); 
    
    vTaskStartScheduler();
}
