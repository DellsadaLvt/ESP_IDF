#include "user_https_request.h"

static const char *TAG = "user_https_request";

void http_request(char *url, char *request, char *buff, uint16_t size_buff){
    // establish connection
    esp_tls_cfg_t cfg = {
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
    struct esp_tls *tls = esp_tls_conn_http_new(url, &cfg);

    if(tls != NULL) {
        ESP_LOGI(TAG, "Connection established...");
    } else {
        ESP_LOGE(TAG, "Connection failed...");
        goto exit;
    }

    // Send command
    int ret= 0u;
    ret = esp_tls_conn_write(tls, request, strlen(request));
    if (ret >= 0) {
        ESP_LOGI(TAG, "%d bytes written", ret);
    } else if (ret != ESP_TLS_ERR_SSL_WANT_READ  && ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
        ESP_LOGE(TAG, "esp_tls_conn_write  returned 0x%x", ret);
        goto exit;
    }

    // Read response
    if( size_buff > 0u){
        ESP_LOGI(TAG, "\nReading HTTP response...\n"); 
        memset(buff, '\0', size_buff);
        ret = esp_tls_conn_read(tls, (char *)buff, size_buff -1u);
        printf("the buff:--ret; %d\n", ret);
        if(ret == ESP_TLS_ERR_SSL_WANT_WRITE  || ret == ESP_TLS_ERR_SSL_WANT_READ){
            ESP_LOGW(TAG, "ESP_TLS_ERR_SSL_WANT_WRITE\n");
        }
        if(ret < 0){
            ESP_LOGE(TAG, "esp_tls_conn_read  returned -0x%x", -ret);
        }
        if(ret == 0){
            ESP_LOGI(TAG, "connection closed");
        }
    }
    
    exit:
        esp_tls_conn_delete(tls);
}



