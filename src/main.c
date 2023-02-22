#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi/wifi.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "server/server.h"
#include "driver/gpio.h"
#include "interface/ssd1306.h"
#define TICK_RATE_MS (1000 / portTICK_PERIOD_MS)

void app_main()
{
    bool i2c_init = false;
    // Pins to output
    esp_err_t sec_pin_status = gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT);
    if (sec_pin_status != ESP_OK)
    {
        while (1)
        {
        }
    }

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    i2c_master_init();
    wifi_init_sta();
    ssd1306_init();
    int level = gpio_get_level(GPIO_NUM_32);
    if (level == 0)
    {
        task_ssd1306_display_clear(NULL);
        task_ssd1306_display_text("PIN: OFF");
    }
    else
    {
        task_ssd1306_display_clear(NULL);
        task_ssd1306_display_text("PIN: ON");
    }
    start_webserver();
}
