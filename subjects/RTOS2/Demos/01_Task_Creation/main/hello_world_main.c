#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

void DemoTask(void *param)
{
    printf("Demo Task Running\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void app_main(void)
{
    printf("Hello world!\n");

    xTaskCreate( DemoTask,
                 "Demo Task",
                 1000,
                 NULL,
                 1,
                 NULL);

    vTaskStartScheduler();
}
