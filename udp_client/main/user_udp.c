#include "user_udp.h"

static const char *TAG = "user_udp";
static int sock= 0u;
char addr_str[16u];
static struct sockaddr_in destAddr;

void user_udp_init( char *host, uint16_t port ){
    
    destAddr.sin_addr.s_addr = inet_addr(host);
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(port);
    int addr_family = AF_INET;
    int ip_protocol = IPPROTO_IP;
    inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);

    // Create socket
    sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
    if (sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        // function to break 
    }
    ESP_LOGI(TAG, "Socket created");
}
//export PATH="$PATH:$HOME/esp/xtensa-lx106-elf/bin"
//export IDF_PATH="$HOME/esp/ESP8266_RTOS_SDK"

void udp_send_data(char *payload, uint16_t u16_len_payload, uint8_t flag){
    int err = sendto(sock, payload, u16_len_payload, flag, (struct sockaddr *)&destAddr, sizeof(destAddr));
    if (err < 0) {
        ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
    }
    ESP_LOGI(TAG, "Message sent");
}

void udp_recv_data( char *buff, uint16_t u16_len_buff, uint8_t flag){
    memset(buff, '\0', u16_len_buff);
    struct sockaddr_in sourceAddr; // Large enough for both IPv4 or IPv6
    socklen_t socklen = sizeof(sourceAddr);
    int len = recvfrom(sock, buff, u16_len_buff - 1, flag, (struct sockaddr *)&sourceAddr, &socklen);
    // Error occured during receiving
    if (len < 0) {
        ESP_LOGW(TAG, "recvfrom failed: errno %d", errno);
    }
    // Data received
    else {
        buff[len] = 0; // Null-terminate whatever we received and treat like a string
        ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
        ESP_LOGI(TAG, "%s", buff);
    }
}

void udp_shutdown( void ){
    ESP_LOGE(TAG, "Shutting down socket and restarting...");
    shutdown(sock, 0);
    close(sock);
}


