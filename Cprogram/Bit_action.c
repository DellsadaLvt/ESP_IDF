#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


static void test_0 ( void );
static void test_1( void );
void using_3bit( void );

int main( void ){
    
    //test_1();
    using_3bit();
    
    return 0;
}

static void test_0 ( void )
{
    uint8_t temp= 0u;
    printf("%x\n", temp);
    for( char i= 0u; i< 5u; i++){
        temp ^= 1u;
        temp |= (1<<4u);
        printf("%p\n", temp);
    }
}

static void test_1( void )
{
    uint16_t value = 0u;
    char data[]= "12/45/1";
    char hour, min, set;

    char *token = NULL;
    token = strtok(data, "/");
    if(token){
        value <<= 5u;
        value |= atoi(token);
        //hour = atoi(token);
    }
    token = strtok(NULL, "/");
    if(token){
        value <<= 6u;
        value |= atoi(token);
        //min = atoi(token);
    }
    token = strtok(NULL, "/");
    if(token){
        value <<= 1u;
        value |= atoi(token);
        //set = atoi(token);
    }
    //ESP_LOGI(TAG, "the %d, %d, %d\n", (value>>7u&32u), (value>>1u&64u), (value&1));
    //printf("%d, %d, %d\n", hour, min, set);
    printf("the data: %d", 2144>>5u&32u);

}

void using_3bit( void )
{
    static uint8_t count = 255u;
    static uint8_t count_flag = 0u;
    /* reset bit 4-6 */
    count &= ~(uint8_t)(7u << 4u);

    while( count_flag  < 10 ){
        count_flag++;
        printf("The count: %d\n", (count>>4u)&7u);
        count += (1u << 4u);
        
    }
}







