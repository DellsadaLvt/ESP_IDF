#include <stdio.h>
#include <stdint.h>



int main( void ){
    uint8_t temp= 0u;
    printf("%x\n", temp);
    for( char i= 0u; i< 5u; i++){
        temp ^= 1u;
        temp |= (1<<4u);
        printf("%p\n", temp);
    }

    
    return 0;
}













