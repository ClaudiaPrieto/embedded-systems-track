
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "spi_flash_mmap.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#define ADC_SAMPLES (uint8_t)20
#undef ADC_NEW
int adc_reading = 0;
int adc_sample = 0;
int voltage = 0;

/*
12dB Reference Voltage: 2.8V
*/

void app_main(void)
{
    printf("Microcontrollers 2 ADC Demo!\n");

    //Create ADC Handle
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t adc1_cfg = {
        .unit_id = ADC_UNIT_1,
    };

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc1_cfg, &adc1_handle));

    //Configure ADC
     adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &config));

    //Calibrate ADC, use Curve Fitting method
    adc_cali_handle_t adc1_cali_chan0_handle = NULL;
    adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = ADC_UNIT_1,
            .chan = ADC_CHANNEL_0,
            .atten = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_12,
        };

    adc_cali_create_scheme_curve_fitting(&cali_config, &adc1_cali_chan0_handle);

    while(1)
    {
        adc_reading = 0;
        for (uint8_t i = 0; i < ADC_SAMPLES; i++) 
        {
            adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &adc_sample);
            adc_reading += adc_sample;
            printf("ADC :%d \n", adc_sample);
        }
        adc_reading /= ADC_SAMPLES;
#ifdef ADC_NEW
         printf("ADC raw to Voltage\n");
        //Convert adc_reading to voltage in mV
        adc_cali_raw_to_voltage(adc1_cali_chan0_handle, adc_reading, &voltage);
#else
       
        printf("ADC formula!\n");
        voltage = (2800 * adc_reading)/4095;
#endif        
        printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}