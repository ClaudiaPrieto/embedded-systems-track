/* General Purpose Timer example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/timer.h"

#define TIMER_DIVIDER         (16)  //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds

uint8_t timergroup0trigger = 0;
uint8_t timergroup1trigger = 0;

// Interrupt every 3s
static void IRAM_ATTR timer_group0_isr_callback(void *args)
{
    timergroup0trigger = 1;
}

// Interrupt every 500 ms
static void IRAM_ATTR timer_group1_isr_callback(void *args)
{
    timergroup1trigger = 1;
}

void app_main(void)
{
     printf("Microcontrollers 2 Timer Demo!\n");

    /* General Timer Configuration*/

    /* Select and initialize basic parameters of the timer */
    timer_config_t config = {
        .divider = TIMER_DIVIDER,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = true,
    }; // default clock source is APB

    /* Timer 0 Group 0 Configuration*/
    timer_init(TIMER_GROUP_0, TIMER_0, &config);

    /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);

    /* Configure the alarm value and the interrupt on alarm. */
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 15000000);
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);

    timer_isr_callback_add(TIMER_GROUP_0, TIMER_0, timer_group0_isr_callback, NULL, 0);

    /* Timer 0 Group 1 Configuration*/
    timer_init(TIMER_GROUP_1, TIMER_0, &config);

    /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(TIMER_GROUP_1, TIMER_0, 0);

    /* Configure the alarm value and the interrupt on alarm. */
    timer_set_alarm_value(TIMER_GROUP_1, TIMER_0, 2500000);
    timer_enable_intr(TIMER_GROUP_1, TIMER_0);

    timer_isr_callback_add(TIMER_GROUP_1, TIMER_0, timer_group1_isr_callback, NULL, 0);

    /* Start Timers */
    timer_start(TIMER_GROUP_0, TIMER_0);  
    timer_start(TIMER_GROUP_1, TIMER_0);

    while (1) {

        if(timergroup0trigger)
        {
             printf("Timer Group 0 Alarm!!\n");
             timergroup0trigger = 0;
        }      
        
        if(timergroup1trigger)
        {
             printf("Timer Group 1 Alarm!!\n");
             timergroup1trigger = 0;
        }      
    }
}
