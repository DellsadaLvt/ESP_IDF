#include "http_server_app.h"

static const char *TAG = "HTTP app server.c";
static httpd_handle_t server = NULL;

extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");

post_data_t wifi_inf_handler = NULL;

httpd_req_t *REQ;

/* HOME page handler */
static esp_err_t home_get_handler(httpd_req_t *req){
    /* send http page */
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start, index_html_end - index_html_start);
    return ESP_OK;
}

static httpd_uri_t home = {
    .uri       = "/home",
    .method    = HTTP_GET,
    .handler   = home_get_handler,
    .user_ctx  = NULL
};

/* Post wifi inf handler */
static esp_err_t wifi_inf_post_handler(httpd_req_t *req){
    char buf[100];
    memset(buf, 0, 100);
    httpd_req_recv(req, buf, req->content_len);
    wifi_inf_handler(buf, strlen(buf));
    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static httpd_uri_t post_data_wifi_inf = {
    .uri       = "/wifi_inf",
    .method    = HTTP_POST,
    .handler   = wifi_inf_post_handler,
    .user_ctx  = NULL
};

/*======================== user function ======================*/

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &home);
        httpd_register_uri_handler(server, &post_data_wifi_inf);
    }
    else{
        ESP_LOGI(TAG, "Error starting server!");
    }
}

void stop_webserver(void){
    // Stop the httpd server
    httpd_stop(server);
}

void set_wifi_inf_callback( void *cb ){
    wifi_inf_handler = cb;
}


