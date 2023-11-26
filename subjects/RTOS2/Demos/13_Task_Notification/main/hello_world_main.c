#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

TimerHandle_t xAutoReloadTimer;
TaskHandle_t DemoTaskHandle = NULL;


static void prvAutoReloadTimerCallback( TimerHandle_t xTimer )
{
    printf( "Timer Sending Notification! \n");
    xTaskNotifyGive(DemoTaskHandle);
    xTaskNotifyGive(DemoTaskHandle);
}

void DemoTask(void *param)
{
    uint32_t ulEventsToProcess;
    while(1)
    {
        ulEventsToProcess = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        
        while( ulEventsToProcess > 0 )
        {
            printf( "Demo Task Processing Event\r\n" );
            ulEventsToProcess--;
        }
    }
}

void app_main(void)
{
    printf("RTOS Task Notification Demo !\n");

    xTaskCreate( DemoTask,
                "Demo Task",
                 1000,
                 NULL,
                 1,
                 &DemoTaskHandle);

    xAutoReloadTimer = xTimerCreate(
        "AutoReload",
        pdMS_TO_TICKS(2000), 
        pdTRUE,
        0,
        prvAutoReloadTimerCallback );

    if(xAutoReloadTimer != NULL)
    {
        xTimerStart( xAutoReloadTimer, 0 );
    }

}
