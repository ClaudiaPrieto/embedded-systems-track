#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

TimerHandle_t xAutoReloadTimer, xOneShotTimer;

static void prvOneShotTimerCallback( TimerHandle_t xTimer )
{
    printf( "One-shot timer callback executing \n");
}


static void prvAutoReloadTimerCallback( TimerHandle_t xTimer )
{
    printf( "Auto-reloadtimer callback executing \n");
}

void app_main(void)
{
    printf("RTOS SW Timers usage Demo !\n");

    xOneShotTimer = xTimerCreate(
        "OneShot",
        pdMS_TO_TICKS(5000), 
        pdFALSE,
        0,
        prvOneShotTimerCallback );
    
    xAutoReloadTimer = xTimerCreate(
        "AutoReload",
        pdMS_TO_TICKS(2000), 
        pdTRUE,
        0,
        prvAutoReloadTimerCallback );

    
    if(xOneShotTimer != NULL)
    {
        xTimerStart( xOneShotTimer, 0 );
    }

    if(xAutoReloadTimer != NULL)
    {
        xTimerStart( xAutoReloadTimer, 0 );
    }
}
