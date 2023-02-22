#include "server.h"
#include "string.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "http_parser.h"
#include "esp_http_server.h"
#include "driver/gpio.h"
#include "interface/ssd1306.h"

esp_err_t disablepin_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    int level = gpio_get_level(GPIO_NUM_32);
    // LOG variable level with ESPLOGI
    ESP_LOGI("DISABLE_PIN", "level: %d", level);

    if (level == 1)
    {
        gpio_set_level(GPIO_NUM_32, 0);
        char resp[] = "{\"status\": \"success"
                      "\"}";
        httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
        task_ssd1306_display_clear(NULL);
        task_ssd1306_display_text("PIN: OFF");
        return ESP_OK;
    }
    if (level == 0)
    {
        gpio_set_level(GPIO_NUM_32, 0);
        char resp[] = "{\"message\": \"already off"
                      "\"}";

        httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
        task_ssd1306_display_clear(NULL);
        task_ssd1306_display_text("PIN: OFF");

        ESP_LOGI("tag", "disablepin_handler called");

        return ESP_OK;
    }
    else
    {
        char resp[] = "{\"message\": \"pin is has unknown status"
                      "\"}";
        httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }
}
/* Our URI handler function to be called during GET /uri request */
esp_err_t enablepin_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

    int level = gpio_get_level(GPIO_NUM_32);
    if (level == 1)
    {
        char resp[] = "{\"message\": \"pin is already on"
                      "\"}";
        httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }
    if (level == 0)
    {
        gpio_set_level(GPIO_NUM_32, 1);
        char resp[] = "{\"message\": \"pin turned on"
                      "\"}";

        httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
        task_ssd1306_display_clear(NULL);
        task_ssd1306_display_text("PIN: ON");
        ESP_LOGI("tag", "enablepin_handler called");

        return ESP_OK;
    }
    else
    {
        char resp[] = "{\"message\": \"pin is has unknown status"
                      "\"}";
        httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }

    /* Send a simple response */

    // Send json response
}

/* URI handler structure for GET /uri */
httpd_uri_t uri_enable = {
    .uri = "/enable",
    .method = HTTP_GET,
    .handler = enablepin_handler,
    .user_ctx = NULL};

httpd_uri_t uri_disable = {
    .uri = "/disable",
    .method = HTTP_GET,
    .handler = disablepin_handler,
    .user_ctx = NULL};

/* Function for starting the webserver */
httpd_handle_t start_webserver(void)
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK)
    {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &uri_enable);

        httpd_register_uri_handler(server, &uri_disable);
    }
    /* If server failed to start, handle will be NULL */
    return server;
}

/* Function for stopping the webserver */
void stop_webserver(httpd_handle_t server)
{
    if (server)
    {
        /* Stop the httpd server */
        httpd_stop(server);
    }
}