#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/event_groups.h"

/* Definitions for the event bits in the event group. */
#define MEASURE_BIT ( 1UL << 0UL ) /* Event bit 0, which is set by a task. */
#define TIMER_BIT   ( 1UL << 1UL ) /* Event bit 1, which is set by a timer. */

TimerHandle_t xAutoReloadTimer;
EventGroupHandle_t xEventGroup;


static void prvAutoReloadTimerCallback( TimerHandle_t xTimer )
{
    printf("Timer Running \n");
    xEventGroupSetBits( xEventGroup, TIMER_BIT);
}

void Measure_Task(void *param)
{
    uint8_t measure_count = 0;
    while(1)
    {
        measure_count++;
        printf("Measure Task Running\n");
        if(measure_count >=5)
        {
            printf("Measurement complete!\n");
            xEventGroupSetBits( xEventGroup, MEASURE_BIT);
            measure_count = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void UI_Task(void *param)
{
    EventBits_t xEventGroupValue;

    while(1)
    {
        printf("UI Task Running and Waiting for events! \n");
        // Wait forever for bits to be set, clear after exit.
        xEventGroupValue = xEventGroupWaitBits(xEventGroup, (MEASURE_BIT | TIMER_BIT), pdTRUE, pdTRUE, portMAX_DELAY);

        printf("UI Task events processed\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    printf("RTOS 2 Event Groups Demo!\n");

    xEventGroup = xEventGroupCreate();

    xAutoReloadTimer = xTimerCreate(
        "AutoReload",
        pdMS_TO_TICKS(5000), 
        pdTRUE,
        0,
        prvAutoReloadTimerCallback );

    if(xAutoReloadTimer != NULL)
    {
        xTimerStart( xAutoReloadTimer, 0 );
    }

    xTaskCreate( Measure_Task,
                 "Measure Task",
                 2000,
                 NULL,
                 2,
                 NULL);

    xTaskCreate( UI_Task,
                 "User Interface",
                 2000,
                 NULL,
                 3,
                 NULL);
}
