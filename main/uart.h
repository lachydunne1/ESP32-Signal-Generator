#ifndef uart
#define uart

#include "esp_err.h"
#include "esp_vfs_dev.h"

esp_err_t init_stdin_stdout(void);
void uart_event_task(void *pvParameters);

#endif //uart.h