#include <string.h>
#include "driver/ledc.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (15) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (4096) // Set duty to 50%. (2 ** 13) * 50% = 4096

/* use designated intializers to init PWM, this should fully enable output*/
void init_ledc(int frequency){
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = frequency,  
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = LEDC_DUTY, //countermax = 2^ledc_timer*dutyres-1 , dutycycle = frac of counter period
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void change_sw_frequency(int freq){
    ESP_ERROR_CHECK(ledc_set_freq(LEDC_MODE, LEDC_TIMER, freq));
}
void user_update_sw_frequency(void){
    printf("\nEnter SW frequency: ");
    char buffer[12];
   
    if(fgets(buffer, sizeof(buffer), stdin)){
        buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline
        int freq = atoi(buffer);
        ESP_ERROR_CHECK(ledc_set_freq(LEDC_MODE, LEDC_TIMER, freq));
        printf("\nAttempted SW frequency update: %i", freq);
    }

}
