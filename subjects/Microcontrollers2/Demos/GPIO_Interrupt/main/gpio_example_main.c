
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define GPIO6_BITMASK (1UL << GPIO_NUM_6)
#define GPIO7_BITMASK (1UL << GPIO_NUM_7)
#define BUTTON_PRESSED 0UL
#define ESP_INTR_FLAG_DEFAULT 0

volatile uint8_t intCout = 0;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    //Disable interrupt to prevent multiple triggers
    gpio_intr_disable(GPIO_NUM_7);
    intCout++;
}

void app_main(void)
{
    uint8_t mainCount = intCout;
    gpio_config_t io_conf = {};

    //interrupt of falling edge
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    //Select GPIO7
    io_conf.pin_bit_mask = GPIO7_BITMASK;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    //disable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_NUM_7, gpio_isr_handler, NULL);

    while(1) 
    {
        if(mainCount != intCout)
        {
            printf("Interrupt Counter: %d\n", intCout);
            mainCount = intCout;
            // Enable interrupt
            gpio_intr_enable(GPIO_NUM_7);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
