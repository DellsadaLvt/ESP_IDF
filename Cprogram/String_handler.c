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

char g_str[100u];

/* Prototype func */
void seperate_string( void );
void string_value( void );
void input_strig( char *input);
rgb_color_t x_seperate_color( char *rgb_color );
void str_input( uint32_t *data );
void copy_str(char *taget);
char* copy_str_1( void );
void combine_str( char *dat );
void test_handler_str( char *str );
void test_str_copy_str( char *data );
void get_str_end( char *data_in, char *data_out, uint8_t n );
void strcat_2_func1( void );
void strcat_2_func2( void );
void test_seperate_str( void );


int main( void ){
    // /* ex_0 */
    // rgb_color_t color = {0};
    // color = x_seperate_color( "CDXF64" );
    // if( color.state == FUNC_OK ){
    //     printf("function oke\n");
    //     printf("The result: %d, %d, %d\n", color.r, color.g, color.b);
    // }
    // else{
    //     printf("function err\n");
    //     printf("The result: %d, %d, %d\n", color.r, color.g, color.b);
    // }

    // /* ex_1 */
    // printf("--------------------------------");
    // char str[]= "string_handler";
    // str_input( (uint32_t*)str );

    // /* ex_2 */
    // char data[10];
    // copy_str(data);
    // printf("Copy data: %s\n", data);

    // /* ex_3 */
    // char *dat;
    // dat = copy_str_1();
    // printf("The data test: %s\n", dat);

    // /* ex_04 */
    // printf("----------ex_04----------\n");
    // char data_m[]= "---data in main---";
    // combine_str(data_m);

    // /* ex_05 */
    // printf("----------ex_05----------\n");
    // char xStr[]= "func /in /ex_05/";
    // printf("Str source before handler: %s\n", xStr);
    // test_handler_str(xStr);
    // printf("Str source after handler: %s\n", xStr);

    // /* ex_06 */
    // printf("----------ex_06----------\n");
    // char xxStr[]= "hello world---------";
    // test_str_copy_str(xxStr);
    
    /* ex_07 */
    // printf("----------ex_07----------\n");
    // char str_in[]= "Le Tu_2.4G#@@#@17012005#@@#@192.168.1.5", str_out[100];
    // get_str_end(str_in, str_out, 11u);
    // printf("--> the ip: %s\n", str_out);

    /* ex_08 */
    // printf("----------ex_08----------\n");
    // strcat_2_func1();
    // printf("---------> %s\n", g_str);
    // strcat_2_func2();
    // printf("---------> %s\n", g_str);


    /* ex_09 */
    printf("----------ex_09----------\n");
    test_seperate_str();


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

void copy_str(char *taget){
    char str[10] = "test--->";
    sprintf(taget, "%s", str);
}

char* copy_str_1( void )
{
    static char str[100]= "Test str pointer --->";
    return str;
}

void combine_str( char *dat )
{
    char data[33u];
    sprintf(data, "%s", dat);
    printf("The data after add: %s\n", data);

    memset(data, 0u, strlen(data));
    sprintf(data, "%s", dat);
    printf("The data after add: %s\n", data);

    char buff[] = "abc_abc_abc";
    strcat(data, buff);
    strcat(data, "@#$!$");
    strcat(data, dat);
    printf("The data after handler: %s\n", data);


}

void test_handler_str( char *str )
{
    char buff[20u]= "";
    sprintf(buff, "%s", str);
    char *token = NULL;

    token = strtok(buff, "/");
    if( token ){
        sprintf(buff, "%s", token);
        printf("The data in func: %s\n", buff);
    }
    token = strtok(NULL, "/");
    if( token ){
        sprintf(buff, "%s", token);
        printf("The data in func: %s\n", buff);
    }
    token = strtok(NULL, "/");
    if( token ){
        sprintf(buff, "%s", token);
        printf("The data in func: %s\n", buff);
    }
}

void test_str_copy_str( char *data )
{
    char temp[6u];
    memset(temp, '\0', sizeof(temp));
    printf("The strlen: %d, the sizeof: %d\n", strlen(temp), sizeof(temp));
    // memset(temp, '\0', )
    strncpy(temp, data, 5u);
    printf("The data using strncpy: %s\n", temp);

    char buff[3u];
    sprintf(buff, "%s", data);
    printf("The buff in subroutine: %s\n", buff);
}

void get_str_end( char *data_in, char *data_out, uint8_t n )
{
    // //char str[]= "Le Tu_2.4G#@@#@17012005#@@#@192.168.1.5";
    memset(data_out, '\0', sizeof(data_out));
    char dest_index = 0u;
    uint16_t len = strlen(data_in);
    for( char i= len-11u; i< len; i++){
        data_out[dest_index] = data_in[i];
        dest_index++;
    }
}

void strcat_2_func1( void )
{
    char ssid[]= "abcasdadfdff";
    strcat(g_str, ssid);
}

void strcat_2_func2 ( void )
{
    char pass[]= "13131434";
    strcat(g_str, pass);
}

void test_seperate_str( void )
{
    char data[300u]= "afasdfds/134132/";
    printf("The ssid: %s, pass: %s\n", strtok(data, "/"), strtok(NULL, "/"));
}

















