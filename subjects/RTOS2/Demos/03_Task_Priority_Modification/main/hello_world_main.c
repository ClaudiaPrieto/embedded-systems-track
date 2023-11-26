#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

TaskHandle_t Task2Handle = NULL;

void Task1(void *param)
{
    UBaseType_t task1Priority;
    while(1)
    {
        task1Priority = uxTaskPriorityGet( NULL );
        printf("Task 1 Running with priority:%d\n",task1Priority );
        vTaskPrioritySet(Task2Handle,3);
    }
}

void Task2(void *param)
{
    UBaseType_t task2Priority;
    while(1)
    {
        task2Priority = uxTaskPriorityGet( NULL );
        printf("Task 2 Running with priority:%d\n",task2Priority );
        // Since task 2 is calling this function, it could have used "NULL"
        // instead of a valid task handle.
        vTaskPrioritySet(Task2Handle,1);
    }
}

void app_main(void)
{
    printf("RTOS 2 Task Priority Demo!\n");

    xTaskCreate( Task2,
                 "Task 2",
                 2000,
                 NULL,
                 1,
                 &Task2Handle);

    xTaskCreate( Task1,
                 "Task 1",
                 2000,
                 NULL,
                 2,
                 NULL);
}
