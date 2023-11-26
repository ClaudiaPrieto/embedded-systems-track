#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

QueueHandle_t xQueue = 0;

typedef enum
{
    temperature,
    humidity
} DataSource_t;

typedef struct
{
    uint8_t Value;
    DataSource_t DataSource;
} Data_t;

void TemperatureTask(void *param)
{
    Data_t lValueToSend;
    BaseType_t xStatus;

    lValueToSend.DataSource = temperature;
    lValueToSend.Value = 25;
    while(1)
    {
        printf("Writing %d to queue \n", lValueToSend.Value);
        xStatus = xQueueSend( xQueue, &lValueToSend, 0 );
        if( xStatus != pdPASS )
        {
            printf("Failed to write:%d to queue!!\n", lValueToSend);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void HumidityTask(void *param)
{
    Data_t lValueToSend;
    BaseType_t xStatus;

    lValueToSend.DataSource = humidity;
    lValueToSend.Value = 15;

    while(1)
    {
        printf("Writing %d to queue\n", lValueToSend.Value);
        xStatus = xQueueSend( xQueue, &lValueToSend, 0 );
        if( xStatus != pdPASS )
        {
            printf("Failed to write:%d to queue!!\n", lValueToSend);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void ReaderTask(void *param)
{
    Data_t lReceivedValue;
    BaseType_t xStatus;

    while (1)
    {
        xStatus = xQueueReceive( xQueue, &lReceivedValue, portMAX_DELAY );
    
        if( xStatus == pdPASS )
        {
            if(lReceivedValue.DataSource == temperature)
            {
                printf("Received %d from temperature thread!!\n", lReceivedValue.Value);
            }
            else if (lReceivedValue.DataSource == humidity)
            {
                printf("Received %d from humidity thread!!\n", lReceivedValue.Value);
            }
            else
            {
                printf("Read %d from the unknown thread !!\n",lReceivedValue.Value);
            }
        }
        else
        {
            printf("Failed to read from queue!!\n");
        }
    }
}


void app_main(void)
{
    printf("RTOS Queue with multiple threads usage Demo !\n");

    xQueue = xQueueCreate( 5, sizeof( Data_t ) );

    if(xQueue != NULL)
    {
        xTaskCreate( TemperatureTask,
                 "Temperature",
                 2000,
                 NULL,
                 1,
                 NULL);
        
        xTaskCreate( HumidityTask,
                 "Humidity",
                 2000,
                 NULL,
                 1,
                 NULL);
        
        xTaskCreate( ReaderTask,
                 "Reader",
                 2000,
                 NULL,
                 2,
                 NULL);
    }
    else
    {
        printf("Failed to create queue!!");
    }

}
