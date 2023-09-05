
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define ADC_SAMPLES (uint8_t)20
#define ADC_NEW
uint32_t adc_reading = 0;
uint32_t voltage = 0;
static esp_adc_cal_characteristics_t *adc_chars;


/*
11dB Reference Voltage: 2.8V
*/

void app_main(void)
{
    printf("Microcontrollers 2 ADC Demo!\n");
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_11);
    adc1_config_width(ADC_WIDTH_BIT_12);

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, adc_chars); 

    while(1)
    {
        adc_reading = 0;
        for (uint8_t i = 0; i < ADC_SAMPLES; i++) 
        {
            adc_reading += adc1_get_raw(ADC1_CHANNEL_0);
        }
        adc_reading /= ADC_SAMPLES;
#ifdef ADC_NEW
         printf("ADC raw to Voltage\n");
        //Convert adc_reading to voltage in mV
        voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
#else
       
        printf("ADC formula!\n");
        voltage = (2800 * adc_reading)/4095;
#endif        
        printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}