#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"


#include "driver/dac_types.h"
#include "driver/dac_cosine.h"  // Include the correct header file
#include <string.h>
#define DAC_CHAN DAC_CHAN_0
#define DAC_ATTEN  DAC_COSINE_ATTEN_DB_6 // 1/2
#define DAC_CLK DAC_COSINE_CLK_SRC_DEFAULT //only option
#define DAC_PHASE DAC_COSINE_PHASE_180 // only does 180 or 0

float dc_offset = 2.5;
dac_cosine_handle_t cos_handle; 

//pass frequency as an argument so that we can easily update externally.
void init_dac_cos(double DAC_FREQ){

    dac_cosine_config_t cos_cfg = {
        .chan_id = DAC_CHAN,
        .freq_hz = DAC_FREQ,
        .clk_src = DAC_CLK,
        .atten = DAC_ATTEN,
        .offset = dc_offset,
        
        .flags = {
            .force_set_freq = true
        }
    };

    //init new error flag
    esp_err_t err = dac_cosine_new_channel(&cos_cfg, &cos_handle);
    if (err != ESP_OK){
        printf("Failed to create DAC cosine channel: %d\n", err);
        return;
    }

    //begin output
    err = dac_cosine_start(cos_handle);
    if (err != ESP_OK) {
        printf("Failed to start DAC cosine output: %d\n", err);
        //can delete if fails
        dac_cosine_del_channel(cos_handle);
    }
}

void invert_dac_output(){
    /* this sets inversion settings on wave form, 
        00- invert any bits,
        01- inverts all bits
        10- inverts MSB
        11- inverts bits except for MSB
     */
    SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV1, 2, SENS_DAC_INV1_S);
}

void stop_and_cleanup_dac_cosine_wave(dac_cosine_handle_t cos_handle) {
    //stops output

    esp_err_t err = dac_cosine_stop(cos_handle);
    if (err != ESP_OK){
        printf("Failed to stop DAC cosine output: %d\n", err);
    }

    //delete the cosine channel
    err = dac_cosine_del_channel(cos_handle);
    if (err!= ESP_OK){
        printf("Failed to delete DAC cosine channel: %d\n", err);
    }
}

void change_dac_frequency(int frequency) {
    
    stop_and_cleanup_dac_cosine_wave(cos_handle);
    init_dac_cos(frequency);
    invert_dac_output();

}

/* SM logic to change DAC freq through UART*/
void toggle_dac_frequency(void){

    char buffer[100];
    //gets is unsafe
    if(fgets(buffer, sizeof(buffer), stdin)){
        buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline
        int freq = atoi(buffer);
        change_dac_frequency(freq);
        printf("Attempted frequency update: %i\n", freq);
    }

}
