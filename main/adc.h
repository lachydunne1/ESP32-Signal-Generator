#ifndef adc 
#define adc

//ADC channel 8 for devkit v1... 
#define ADC_SELECT ADC_CHANNEL_0
#define SAMPLING_FREQ 17500

void adc_init(void);
float read_pin(void);
void readPinTask(void);
#endif //adc.h