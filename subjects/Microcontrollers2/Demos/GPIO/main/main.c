

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "esp_rom_gpio.h"

#define GPIO6_BITMASK (1UL << GPIO_NUM_6)
#define GPIO7_BITMASK (1UL << GPIO_NUM_7)
#define BUTTON_PRESSED 0UL

uint8_t button_pressed = 0;

uint8_t ledLevel = 0;

void app_main(void)
{
    //zero-initialize the config structure.

    // Initialize output pin through method #1: configuration structure
    gpio_config_t io_conf = {0};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
     //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    //disable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    //Select GPIO6
    io_conf.pin_bit_mask = GPIO6_BITMASK;

    //configure GPIO with the given settings
    gpio_config(&io_conf);

    // Initialize input pin through method #2: APIs
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_7);
    gpio_set_direction(GPIO_NUM_7, GPIO_MODE_INPUT);
    gpio_pulldown_dis(GPIO_NUM_7);
    gpio_pullup_en(GPIO_NUM_7);
    gpio_set_intr_type(GPIO_NUM_7, GPIO_INTR_DISABLE);

    while(1)
    {
        if(gpio_get_level(GPIO_NUM_7) == BUTTON_PRESSED)
        {
            printf("Button Pressed! \n");
            gpio_set_level(GPIO_NUM_6, 1); 
        }
        else
        {
            printf("Button Released! \n");
            gpio_set_level(GPIO_NUM_6, 0); 
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
