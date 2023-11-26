#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

TaskHandle_t Task2Handle = NULL;

void Task1(void *param)
{
    while(1)
    {
        printf("Task 1 Running \n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void Task2(void *param)
{
    uint8_t runCounter = 0;
    while(1)
    {
        printf("Task 2 Ran for:%d times\n", runCounter);
        runCounter++;
        if(runCounter>=10)
        {
            // Since task 2 is calling this function, it could have used "NULL"
            // instead of a valid task handle.
            vTaskDelete(Task2Handle);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    printf("RTOS 4 Task Deletion Demo!\n");

    xTaskCreate( Task1,
                 "Task 1",
                 2000,
                 NULL,
                 1,
                 NULL);

    xTaskCreate( Task2,
                 "Task 2",
                 2000,
                 NULL,
                 1,
                 &Task2Handle);
}
