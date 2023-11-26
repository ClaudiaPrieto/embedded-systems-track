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
#define TEMP_MEASURE_BIT ( 1UL << 0UL ) /* Event bit 0, which is set by temp. measurement task. */
#define HUM_MEASURE_BIT  ( 1UL << 1UL ) /* Event bit 1, which is set by hum. measurement task. */
#define UI_UPDATE_BIT    ( 1UL << 2UL ) /* Event bit 2, which is set by UI task. */

EventGroupHandle_t xEventGroup;


void Temp_Measure_Task(void *param)
{
    uint8_t measure_count = 0;
    while(1)
    {
        measure_count++;
        printf("Temp. Measurement Task Running\n");
        if(measure_count >=5)
        {
            printf("Temp. Measurement complete!\n");
            xEventGroupSync(  xEventGroup, TEMP_MEASURE_BIT, (TEMP_MEASURE_BIT|HUM_MEASURE_BIT| UI_UPDATE_BIT ),portMAX_DELAY );
            measure_count = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void Hum_Measure_Task(void *param)
{
    uint8_t measure_count = 0;
    while(1)
    {
        measure_count++;
        printf("Hum. Measurement  Task Running\n");
        if(measure_count >=5)
        {
            printf("Hum. Measurement complete!\n");
            xEventGroupSync(  xEventGroup, HUM_MEASURE_BIT, (TEMP_MEASURE_BIT|HUM_MEASURE_BIT| UI_UPDATE_BIT ),portMAX_DELAY );
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
        printf("UI Task Running \n");

        xEventGroupSync(  xEventGroup, UI_UPDATE_BIT, (TEMP_MEASURE_BIT|HUM_MEASURE_BIT| UI_UPDATE_BIT ),portMAX_DELAY );

        printf("UI Task events processed\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    printf("RTOS 2 Event Group Sync Demo!\n");

    xEventGroup = xEventGroupCreate();


    xTaskCreate( Temp_Measure_Task,
                 "Temp Measure Task",
                 2000,
                 NULL,
                 2,
                 NULL);

    xTaskCreate( Hum_Measure_Task,
                 "Hum Measure Task",
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
