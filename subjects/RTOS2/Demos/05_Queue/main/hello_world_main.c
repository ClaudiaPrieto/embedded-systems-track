#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

QueueHandle_t xQueue = 0;

void SenderTask(void *param)
{
    int32_t lValueToSend;
    BaseType_t xStatus;

    lValueToSend = ( int32_t )param;

    while(1)
    {
        printf("Writing %d to queue \n", lValueToSend);
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
    int32_t lReceivedValue;
    BaseType_t xStatus;

    while (1)
    {
        xStatus = xQueueReceive( xQueue, &lReceivedValue, portMAX_DELAY );
    
        if( xStatus == pdPASS )
        {
            printf("Read %d from the queue!!\n", lReceivedValue);
        }
        else
        {
            printf("Failed to read from queue!!\n");
        }
    }
}


void app_main(void)
{
    printf("RTOS 2 Queue usage Demo !\n");

    xQueue = xQueueCreate( 5, sizeof( int32_t ) );

    if(xQueue != NULL)
    {
        xTaskCreate( SenderTask,
                 "Sender",
                 2000,
                 (void *) 100,
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
