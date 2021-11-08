#include "user_udp_server.h"

static int sock= 0;
static struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
static socklen_t socklen = sizeof(source_addr);
static const char *TAG = "user_udp_server";

esp_err_t user_udp_init(uint16_t port){
    struct sockaddr_in6 dest_addr;
    struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
    dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr_ip4->sin_family = AF_INET;
    dest_addr_ip4->sin_port = htons(port);
    uint8_t ip_protocol = IPPROTO_IP;

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, ip_protocol);
    if (sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Socket created");

    // Socket bind
    int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Socket bound, port %d", port);

    return ESP_OK;
}

esp_err_t user_upd_shutdown( void ){
    ESP_LOGE(TAG, "Shutting down socket\n");
    shutdown(sock, 0);
    close(sock);

    return ESP_OK;
}

esp_err_t user_udp_send(char *buffer, uint16_t u16_len_buffer, uint8_t flag){
    int err = sendto(sock, buffer, u16_len_buffer, flag, (struct sockaddr *)&source_addr, sizeof(source_addr));
    if (err < 0) {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t user_udp_recv(char *rx_buffer, uint16_t u16_len_buffer, uint8_t flag){
    // Clear buffer
    memset(rx_buffer, '\0', u16_len_buffer);
    int len = recvfrom(sock, rx_buffer, u16_len_buffer - 1, flag, (struct sockaddr *)&source_addr, &socklen);

    // Error occurred during receiving
    if (len < 0) {
        ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);

        return ESP_FAIL;
    }
    // Data received
    else {
        // Get the sender's ip address as string
        char addr_str[16u];
        inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string...
        ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
        ESP_LOGI(TAG, "%s", rx_buffer);
    }

    return ESP_OK;
}
            
           

                

        