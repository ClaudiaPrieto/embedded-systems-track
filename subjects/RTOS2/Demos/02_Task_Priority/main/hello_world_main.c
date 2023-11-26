#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

void Task1(void *param)
{
    while(1)
    {
        printf("Task 1 Running\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void Task2(void *param)
{
    while(1)
    {
        printf("Task 2 Running\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    printf("RTOS 2 Task Priority Demo!\n");

    xTaskCreate( Task1,
                 "Task 1",
                 1000,
                 NULL,
                 2,
                 NULL);

    xTaskCreate( Task2,
                 "Task 2",
                 1000,
                 NULL,
                 1,
                 NULL);
}
