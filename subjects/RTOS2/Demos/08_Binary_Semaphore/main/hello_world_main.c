#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

TimerHandle_t xAutoReloadTimer;
SemaphoreHandle_t xBinarySemaphore;


static void prvAutoReloadTimerCallback( TimerHandle_t xTimer )
{
    printf( "Timer giving semaphore! \n");
    xSemaphoreGive( xBinarySemaphore);
}

void DemoTask(void *param)
{
    while(1)
    {
        xSemaphoreTake( xBinarySemaphore, portMAX_DELAY);
        printf("Demo Task Running\n");
    }
}

void app_main(void)
{
    printf("RTOS Binary Semaphore Demo !\n");

    xTaskCreate( DemoTask,
                "Demo Task",
                 1000,
                 NULL,
                 1,
                 NULL);

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

    xBinarySemaphore = xSemaphoreCreateBinary();
}
