#ifndef dac
#define dac
#include "driver/dac_types.h"
#include "driver/dac_cosine.h"  // Include the correct header file
/* cos handle init here for global access, i think this could be bad */

void init_dac_cos(int frequency);
void stop_and_cleanup_dac_cosine_wave(dac_cosine_handle_t cos_handle);
void change_dac_frequency(int frequency);
void user_update_dac_frequency(void);
void invert_dac_output(void);
#endif //dac.h

