
#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"

#define LEDC_OUTPUT_IO          (6) // Define the output GPIO
#define LEDC_DUTY               (2048) // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
#define LEDC_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz

ledc_timer_config_t ledc_timer = 
{
    .speed_mode       = LEDC_LOW_SPEED_MODE,
    .timer_num        = LEDC_TIMER_0,
    .duty_resolution  = LEDC_TIMER_13_BIT,
    .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
    .clk_cfg          = LEDC_AUTO_CLK
};

ledc_channel_config_t ledc_channel = {
    .speed_mode     = LEDC_LOW_SPEED_MODE,
    .channel        = LEDC_CHANNEL_0,
    .timer_sel      = LEDC_TIMER_0,
    .intr_type      = LEDC_INTR_DISABLE,
    .gpio_num       = LEDC_OUTPUT_IO,
    .duty           = 0, // Set duty to 0%
    .hpoint         = 0
};

void app_main(void)
{
    printf("Micros 2 PWM Demo!");

    // Configure PWM Timer
    ledc_timer_config(&ledc_timer);

    // Configure PWM Channel
    ledc_channel_config(&ledc_channel);

    // Set duty to 50%
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, LEDC_DUTY);

    // Update duty to apply the new value
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

}
