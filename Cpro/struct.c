#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct pro {
    char name[10u];
    uint8_t age;
} profile;

int main( void ){
    profile stua = {
        .name = "nhucy",
        .age = 18u
    };
    printf("The stua: %s, %d\n", stua.name, stua.age );

    profile stub;
    sprintf(stub.name, "%s", stua.name);
    stub.age = stua.age;
    printf("The stub: %s, %d\n", stub.name, stub.age);

    profile stuc;
    sprintf((char*)stuc.name, "%s", "namyca");
    stuc.age = 19u;
    //printf("The stuc: %s, %d\n", stuc->name, stuc->age );
    printf("The stuc: %s, %d\n", stuc.name, stuc.age );

    profile *stud;
    uint8_t a= 20u;
    sprintf((char*)stud->name, "%s", stuc.name);
    stud->age = 22;
    printf("The stud: %s, %d\n",stud->name, stud->age );


    return 0u;
}





