#include "http_server_app.h"

static const char *TAG = "HTTP server";
static httpd_handle_t server = NULL;

/* declare to insert image in flash */
// extern const uint8_t index_html_start[] asm("_binary_image_png_start");
// extern const uint8_t index_html_end[] asm("_binary_image_png_end");

extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");

/* Declare for callback func */
static http_post_callback_t http_post_slider_callback   = NULL;
static http_post_callback_t http_post_switch_callback   = NULL;
static http_post_callback_t http_post_wifi_inf_callback = NULL;
static http_get_callback_t  http_get_dht11_callback     = NULL;
httpd_req_t *REQ;

/* An HTTP GET handler */
esp_err_t hello_get_handler(httpd_req_t *req)
{
    /* Send response with custom headers and body set as the
     * string passed in user context*/
    // const char* resp_str = (const char*)"hello world!";
    // httpd_resp_send(req, resp_str, strlen(resp_str));

    /* send image */
    // httpd_resp_set_type(req, "image/png");
    // httpd_resp_send(req, (const char *)index_html_start, index_html_end - index_html_start);
    
    /* send http page */
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start, index_html_end - index_html_start);
    
    return ESP_OK;
}

httpd_uri_t get_data_dht11 = {
    .uri       = "/dht11",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};

void dht11_response( char *data, uint16_t len){
    httpd_resp_send(REQ, data, len);
}

/* An HTTP GET data dht11 handler */
esp_err_t get_data_dht11_handler(httpd_req_t *req)
{   
    //static uint8_t u8_test_value= 0u;
    //char resp_str[100u];
    /* send data dht11 */
    // u8_test_value+= 20u;
    // sprintf(resp_str, "{\"temperature\": \"%d\", \"humidity\": \"%d\"}", u8_test_value, u8_test_value);
    // httpd_resp_send(req, (const char*)resp_str, strlen(resp_str));
    
    REQ = req;
    /* using callback func */
    http_get_dht11_callback();
    return ESP_OK;
}

httpd_uri_t get_data_dht11_interval = {
    .uri       = "/getdatadht11",
    .method    = HTTP_GET,
    .handler   = get_data_dht11_handler,
    .user_ctx  = NULL
};

/* 404 error handler */
// esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err){
//     if( strcmp("/dht11", req->uri) == 0u){
//         httpd_resp_send_404(req);
//     }
// }

/* An HTTP POST handler */
 esp_err_t echo_post_handler(httpd_req_t *req){
    char buf[100];
    memset(buf, 0, 100);
    httpd_req_recv(req, buf, req->content_len);
    printf("DATA: %s\n", buf);
    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

httpd_uri_t post_data = {
    .uri       = "/post_data",
    .method    = HTTP_POST,
    .handler   = echo_post_handler,
    .user_ctx  = NULL
};


/* An HTTP POST button handler */
 esp_err_t button_post_handler(httpd_req_t *req){
    char buf[100];
    memset(buf, 0, 100);
    httpd_req_recv(req, buf, req->content_len);
    //printf("DATA: %s\n", buf);
    http_post_switch_callback(buf, req->content_len);
    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

httpd_uri_t post_data_button = {
    .uri       = "/switch1",
    .method    = HTTP_POST,
    .handler   = button_post_handler,
    .user_ctx  = NULL
};

/* An HTTP POST slider handler */
 esp_err_t slider_post_handler(httpd_req_t *req){
    char buf[100];
    memset(buf, 0, 100);
    httpd_req_recv(req, buf, req->content_len);
    http_post_slider_callback(buf, req->content_len);
    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

httpd_uri_t post_data_slider = {
    .uri       = "/slider_led",
    .method    = HTTP_POST,
    .handler   = slider_post_handler,
    .user_ctx  = NULL
};

/* An HTTP POST wifi inf handler */
 esp_err_t wifi_inf_post_handler(httpd_req_t *req){
    char buf[100];
    memset(buf, 0, 100);
    httpd_req_recv(req, buf, req->content_len);
    http_post_wifi_inf_callback(buf, req->content_len);
    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

httpd_uri_t post_data_wifi_inf = {
    .uri       = "/wifi_inf",
    .method    = HTTP_POST,
    .handler   = wifi_inf_post_handler,
    .user_ctx  = NULL
};

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &get_data_dht11);
        httpd_register_uri_handler(server, &get_data_dht11_interval);
        httpd_register_uri_handler(server, &post_data);
        httpd_register_uri_handler(server, &post_data_button);
        httpd_register_uri_handler(server, &post_data_slider);
        httpd_register_uri_handler(server, &post_data_wifi_inf);
    }
    else{
        ESP_LOGI(TAG, "Error starting server!");
    }
}

void stop_webserver(void)
{
    // Stop the httpd server
    httpd_stop(server);
}

void http_set_callback_switch(void *cb){
    http_post_switch_callback = cb;
}

void http_set_calback_dht11( void *cb ){
    http_get_dht11_callback = cb;
}

void http_set_callback_slider( void *cb ){
    http_post_slider_callback = cb;
}

void http_set_callback_wifi_inf( void *cb ){
    http_post_wifi_inf_callback = cb;
}



