#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include <sys/time.h>
#include "esp_sleep.h"

#undef SLEEP_DEMO

void app_main(void)
{
    time_t cur_Seconds;
    char* localTime;
    struct tm *cur_Time;
    struct tm tm;
    struct timeval dateandTime = {0};

    printf("Microcontrollers 2 RTC Demo!\n");

    esp_sleep_wakeup_cause_t wakeSource = esp_sleep_get_wakeup_cause();
    printf("Wake source: %d\n", wakeSource);

    if (wakeSource  != ESP_SLEEP_WAKEUP_TIMER)
    {
        // Configure desired date and time
        tm.tm_year = 2023 - 1900; // Year relative to 1900
        tm.tm_mon = 8; //Months since january
        tm.tm_mday = 18; //Day of the month
        tm.tm_hour = 11; //Hours since midnight
        tm.tm_min = 14; //Minutes after the hour
        tm.tm_sec = 10; //Seconds after the minute

        //Get number of seconds from tm structure
        dateandTime.tv_sec =  mktime(&tm);
        
        //Set new date and time
        settimeofday(&dateandTime, NULL);
    }

    while(1)
    {
        //returns current time in seconds
        cur_Seconds= time(NULL);
        //Fill time structure with new seconds
        cur_Time = localtime(&cur_Seconds);
        // Returns time in user understandable string
        localTime = asctime(cur_Time);
        //Print current time
        printf("Current Time: %s \n\r", localTime);

#ifdef SLEEP_DEMO
        printf("Going to sleep\r");
        int64_t sleepTime = 15 *1000 *1000;
        esp_deep_sleep(sleepTime);
        // NEVER RETURNS
#else
        vTaskDelay(pdMS_TO_TICKS(1000));
#endif
    }
}
