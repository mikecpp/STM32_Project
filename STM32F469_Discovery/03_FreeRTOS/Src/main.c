#include "stm32f469_system.h" 
#include "uart.h"
#include "stm32469i_discovery_lcd.h"
#include <stdlib.h>
#include "cmsis_os.h"

#define X_SIZE      800
#define Y_SIZE      480

SemaphoreHandle_t xSemaphore    = NULL;
QueueHandle_t     queue_display = NULL;
TimerHandle_t     timer         = NULL;

int count = 0;
char msg[64];

static void app_semaphore_test(void *param)
{
    while(1) {
        if(xSemaphoreTake(xSemaphore, 10) == pdTRUE) {
            count++;
            if(count % 10 == 0) {
                xQueueSend(queue_display, &count, 0); 
            }
            printf("Count = %d \r\n", count);
            sprintf(msg, "Count = %d", count); 
            BSP_LCD_DisplayStringAtLine(3, (uint8_t*) msg);    
            vTaskDelay(1000);
            xSemaphoreGive(xSemaphore);   
        }
    }
} 

static void main_task(void *param)
{
    xSemaphore    = xSemaphoreCreateMutex();
    queue_display = xQueueCreate(10, sizeof(uint32_t));
      
    for(int i=0; i<2; i++) {
        xTaskCreate(app_semaphore_test, "APP Count 1", 1000, NULL, 5, NULL); 
    }    
    
    uint32_t number;
    char msg[32];
    while(1) {
        if(xQueueReceive(queue_display, &number, 0) == pdTRUE) {
            sprintf(msg, "MSG Count = %d", number);
            BSP_LCD_DisplayStringAtLine(4, (uint8_t*) msg);
        }
        vTaskDelay(100);
    }
}

static void timer_task(TimerHandle_t timer)
{
    int c = 0;
    
    printf("Timer = %d (%X) \r\n", c++, (uint32_t) timer);
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

    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);  
    BSP_LCD_DisplayStringAtLine(1, (uint8_t*) "Hello Mike");
    BSP_LCD_DisplayStringAtLine(2, (uint8_t*) "ABCDE1234567890");        

    timer = xTimerCreate("Timer", 1000, pdTRUE, NULL, timer_task);
    xTimerStart(timer, 0);
    
    xTaskCreate(main_task, "Main Task", 1000, NULL, 5, NULL);
   
    vTaskStartScheduler();
}
