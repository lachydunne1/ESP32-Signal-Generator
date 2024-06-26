#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "soc/rtc_io_reg.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "soc/rtc.h"

#include <sdkconfig.h>
#include <esp_vfs_dev.h>

#include <dac.h>
#include <adc.h>
#include <uart.h>



//init frequency as 5kHz
int freq = 1000;

void err(const char *error);
void bode_inputs(void);
void bode(float voff, float vsigpp, int samples, int frq_start, int frq_stop);
void myTask(void *parameter);

void app_main(){
    
    init_dac_cos(freq);
    init_stdin_stdout();
    invert_dac_output(); /* necessary for sine wave */
    adc_init();
    
    xTaskCreate(myTask, "Arb Task", 1024*3, NULL, 10, NULL);
}

void myTask(void *parameter){

    char input;

    while(1){
        // fix blocking for instantaneous switch
        input = getchar(); // get char is blocking

        switch (input) {

            case 'F':
                toggle_dac_frequency();  
                break;

            case 'B':
                bode_inputs();
                break;

            default:

                break;
        }
    
        //read_temp();
        vTaskDelay(500/portTICK_PERIOD_MS);     
    }
}

void err(const char *message){
    perror(message);
    exit(EXIT_FAILURE);
}

void bode_inputs(){
    /* get user inputs*/
    float voff, vsigpp;
    int samples, frq_start, frq_stop;


    printf("\nEnter sample ammount: ");
    scanf("%d", &samples);

    printf("\nVoltage offset of input signal: ");
    if (scanf("%f", &voff) != 1) {
        fprintf(stderr, "Invalid input. Please enter a valid float value. \n");
        return;
    }

    printf("\nInput signal amplitude: ");
    if (scanf("%f", &vsigpp) != 1) {
        fprintf(stderr, "Invalid input. Please enter a valid float value. \n");
        return;
    }
    
    printf("\nEnter start frequency (Hz): ");
    if (scanf("%d", &frq_start) != 1){
        fprintf(stderr, "Invalid input. Please enter a valid integer value. \n");
    }
        

    printf("\nEnter stop frequency (Hz): ");
    if (scanf("%d", &frq_stop) != 1){
        fprintf(stderr, "Invalid input. Please enter a valid integer value. \n");
    }

    if((voff < 0) || (vsigpp < 0) || (frq_start < 0) || (frq_stop < 0)){
        err("\nNo negative inputs.");
        return;
    }
    bode(voff, vsigpp, samples, frq_start, frq_stop);

}


void bode(float voff, float vsigpp, int samples, int frq_start, int frq_stop){

    printf("Begun bode.\n");
    int step = (frq_stop - frq_start) / samples;

    if (step ==0){
        perror("Step is 0. \n");
        return;
    }

    printf("Step Increment: %i \n Start Frequency: %i \n Stop Frequency: %i \n",step, frq_start, frq_stop);
    /* init reference freq */
    int sample_freq = frq_start;
    float adc_value =0;
    float mag_db;
    
    while(sample_freq < frq_stop){
        //what is the maximum scale of the signal?
        adc_value = read_pin();

        if (adc_value == 0){
            perror("ADC read failure. \n");
            return;

        }else{  

            mag_db = 20 * log(( adc_value - voff) /vsigpp);        
            //once complete step frequency
            printf("| Frequency: %i  | Magnitude: %f | \n", sample_freq, mag_db);
           
            change_dac_frequency(sample_freq);
        }
        sample_freq += step;
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    } 

    printf("Bode complete. \n");
    /* result to original frequency */
    change_dac_frequency(freq);
}