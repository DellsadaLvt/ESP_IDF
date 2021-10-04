#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef enum {
    FUNC_OK,
    FUNC_ERR
} state_func_t;

typedef struct wifi_inf{
    char ssid[32];
    char pass[64];
}wifi_inf;

typedef struct rgb {
    uint8_t r, g, b;
    state_func_t state;
} rgb_color_t;


/* Prototype func */
void seperate_string( void );
void string_value( void );
void input_strig( char *input);
rgb_color_t x_seperate_color( char *rgb_color );
void str_input( uint32_t *data );


int main( void ){
    /* ex_0 */
    rgb_color_t color = {0};
    color = x_seperate_color( "CDXF64" );
    if( color.state == FUNC_OK ){
        printf("function oke\n");
        printf("The result: %d, %d, %d\n", color.r, color.g, color.b);
    }
    else{
        printf("function err\n");
        printf("The result: %d, %d, %d\n", color.r, color.g, color.b);
    }

    /* ex_1 */
    printf("--------------------------------");
    char str[]= "string_handler";
    str_input( (uint32_t*)str );

    return 0;
}


void seperate_string( void ){
    char str[100]= "this is the test @!!$& 12345678";
    const char key[6]= "@!!$&";
    char *token;

    char ssid[30], pass[30];
    token = strtok(str, key);
    printf("The data: %s\n", str);
    sprintf(ssid, "%s", token);
    token = strtok(NULL, key);
    printf("The data: %s\n", str);
    sprintf(pass, "%s", token);
    printf("The ssid: %s\n", ssid);
    printf("The pass: %s\n", pass);
}

void string_value( void ){
    char ssid[32]= "Wifi_ssid", pass[64]= "wifi_pass";
    wifi_inf wifi;
    sprintf(wifi.ssid, "%s", ssid);
    sprintf(wifi.pass, "%s", pass);
    printf("--> ssid: %s\n--> pass: %s\n", wifi.ssid, wifi.pass);
}

void input_strig( char *input){
    char str[100];
    sprintf(str, "%s", input);
    printf("The string: %s\n", str);
}

rgb_color_t x_seperate_color( char *rgb_color ){
    rgb_color_t color= {0};
    char buff[6u];
    sprintf(buff, "%s", rgb_color);
    
    /* convert str_hex to int */
    for(char i= 0u; i < 6u; i++){
        if( buff[i]>= 48 && buff[i] <= 57 ){
            if( i < 2){
                color.r <<= 4u;
                color.r |= ((uint8_t)buff[i] - 48);
            }
            else if ( i < 4){
                color.g <<= 4u;
                color.g |= ((uint8_t)buff[i] - 48);
            }
            else if ( i < 6){
                color.b <<= 4u;
                color.b |= ((uint8_t)buff[i] - 48);
            }
        }
        else if( buff[i]>= 65 && buff[i] <= 70 ){
            if( i < 2){
                color.r <<= 4u;
                color.r |= ((uint8_t)buff[i] - 65 + 10);
            }
            else if ( i < 4){
                color.g <<= 4u;
                color.g |= ((uint8_t)buff[i] - 65 + 10);
            }
            else if ( i < 6){
                color.b <<= 4u;
                color.b |= ((uint8_t)buff[i] - 65 + 10);
            }
        }
        else{
            color.state = FUNC_ERR;
            return color;
        }
    }
    color.state = FUNC_OK;
    return color;
}

void str_input( uint32_t *data ){
    printf("The data: %s\n", data);
    char str[100u];
    sprintf(str,"%s", data); 
    printf("The data after handler: %s\n", str);
}








