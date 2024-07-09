#include <esp_adc/adc_oneshot.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include <stdio.h>

#include "adc.h"
//config adc, PIN 9
#define ADC_SELECT ADC_CHANNEL_0
#define SAMPLING_FREQ 17500

adc_oneshot_unit_handle_t adc_handle;
float vpin = 3.3;

//TODO: Write samples to csv after processing
char *infile = "audio_data.csv";


void adc_init(void){

    adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id = ADC_UNIT_1
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_cfg, &adc_handle));

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_DEFAULT

    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_SELECT, &config));

}

#include <stdio.h>

float read_pin(void) {

    int adc_value;
    float adc_voltage;
    float total_voltage = 0.0f;
    int num_samples = 5;

    for (int i = 0; i < num_samples; i++) {
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_SELECT, &adc_value));
        adc_voltage = (vpin/ 4095.0f) * (float)adc_value ;
        total_voltage += adc_voltage;
    }

    float average_voltage = total_voltage / num_samples;

    return average_voltage;
}

// TODO: Accurately read amplitudes for signals.
/* uhhh? silence the compiler for now*/
float read_signal_amplitude(int fsignal){

    int adc_value;
    float adc_voltage; 
    int n = SAMPLING_FREQ / fsignal; /* equivalent to Tsignal / Tsample */
    float maximum = 0;

    for (int i = 0; i<n; i++){
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_SELECT, &adc_value));
        adc_voltage = (vpin/ 4095.0f) * (float)adc_value ;
        
        if (adc_voltage > maximum){
            maximum = adc_voltage;
        }
        vTaskDelay(pdMS_TO_TICKS(1/(SAMPLING_FREQ * 1e-3)));
    }  

    return maximum;
}
// read ADC pin. Should scale to reference voltage
void readPinTask(void){

    int adc_value;
    float signal;
    
    while(1){

        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_SELECT, &adc_value));
        //microphone signal should be based about 5V.
        signal = (vpin/ 4095.0f) * (float)adc_value;
        printf("%f\n", signal);
        //convert sampling freq to period T, ms to RTOS ticks -> 
        vTaskDelay(pdMS_TO_TICKS(1/(SAMPLING_FREQ * 1e-3)));
        
    }

}