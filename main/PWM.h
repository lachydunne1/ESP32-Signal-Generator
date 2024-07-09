#ifndef PWM
#define PWM
 
void init_ledc(int freq);
void change_sw_frequency(int freq);
void user_update_sw_frequency(void);

#endif //PWM.h