#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "http_parser.h"

enum PIN_STATE
{
    PIN_STATE_OFF,
    PIN_STATE_ON
};
httpd_handle_t start_webserver(void);

esp_err_t post_handler(httpd_req_t *req);
esp_err_t get_handler(httpd_req_t *req);
void stop_webserver(httpd_handle_t server);