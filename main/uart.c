#include "esp_err.h"
#include "esp_vfs_dev.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/uart_select.h"
#include "sdkconfig.h"

#define PATTERN_CHR_NUM (3)
#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)


/* stdio functions typically block*/
esp_err_t init_stdin_stdout(void)
{
    static bool configured = false;
    if (configured) {
      return ESP_OK;
    }
    /*Initialize VFS & UART so we can use stdout,in */
    setvbuf(stdin, NULL, _IONBF, 0);
    /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK( uart_driver_install( (uart_port_t)CONFIG_ESP_CONSOLE_UART_NUM,
            256, 0, 0, NULL, 0) );
    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);
    configured = true;
    return ESP_OK;
}

