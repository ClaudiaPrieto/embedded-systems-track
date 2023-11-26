#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/uart.h"

#define UART_PORT UART_NUM_0
#define UART_TX_PIN 21
#define UART_RX_PIN 20
#define BUF_SIZE (1024)

TimerHandle_t xAutoReloadTimer;
SemaphoreHandle_t printMutex;


char task_text[] = "Task 1 Running\n\r";
char text_Buffer[1000];
uint8_t bufferCount = 0;

static void prvAutoReloadTimerCallback( TimerHandle_t xTimer )
{
    xSemaphoreTake(printMutex, portMAX_DELAY); 
    text_Buffer[bufferCount++] = 'T';
    text_Buffer[bufferCount++] = 'i';
    text_Buffer[bufferCount++] = 'm';
    text_Buffer[bufferCount++] = 'e';
    text_Buffer[bufferCount++] = 'r';
    text_Buffer[bufferCount++] = '\n';
    text_Buffer[bufferCount++] = '\r';
    xSemaphoreGive(printMutex);
}

void Task1(void *param)
{
    while(1)
    {
        xSemaphoreTake(printMutex, portMAX_DELAY); 
        text_Buffer[bufferCount++] = 'T';
        vTaskDelay(pdMS_TO_TICKS(1));
        text_Buffer[bufferCount++] = 'a';
        vTaskDelay(pdMS_TO_TICKS(1));
        text_Buffer[bufferCount++] = 's';
        vTaskDelay(pdMS_TO_TICKS(1));
        text_Buffer[bufferCount++] = 'k';
        vTaskDelay(pdMS_TO_TICKS(1));
        text_Buffer[bufferCount++] = '1';
        vTaskDelay(pdMS_TO_TICKS(1));
        text_Buffer[bufferCount++] = '\n';
        text_Buffer[bufferCount++] = '\r';
        xSemaphoreGive(printMutex);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void Task2(void *param)
{
    while(1)
    {
        uart_write_bytes(UART_PORT, (const char *) text_Buffer, bufferCount);
        bufferCount = 0;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    printf("RTOS 2 Critical Section Demo!\n");

    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    ESP_ERROR_CHECK(uart_driver_install(UART_PORT, BUF_SIZE , BUF_SIZE, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_PORT, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    printMutex = xSemaphoreCreateMutex();

    xAutoReloadTimer = xTimerCreate(
        "AutoReload",
        pdMS_TO_TICKS(200), 
        pdTRUE,
        0,
        prvAutoReloadTimerCallback );

    if(xAutoReloadTimer != NULL)
    {
        xTimerStart( xAutoReloadTimer, 0 );
    }

    xTaskCreate( Task1,
                 "Task 1",
                 2000,
                 NULL,
                 0,
                 NULL);

    xTaskCreate( Task2,
                 "Task 2",
                 2000,
                 NULL,
                 2,
                 NULL);
}
