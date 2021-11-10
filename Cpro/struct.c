#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct pro {
    char name[10u];
    uint8_t age;
} profile;

int test_1( void ){
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


typedef struct cJSON
{
    struct cJSON *next;
    struct cJSON *prev;
    struct cJSON *child;
    int type;
    char *valuestring;
    int valueint;
    double valuedouble;
    char *string;
} cJSON;


void test_2( void ){
    printf("hadler\n");
    cJSON item = {
        .string= "hello",
    };
    char *temp;

    // char buff[50u]= "{\"one\":\"1\",\"two\":\"2\", \"three\":\"3\"}";
    // printf("The buff: %s\n", buff);
    // sprintf(item->string, "%s", "hello");
    // printf("after handler\n");
    printf("The result: %s\n", item.string);
}






int main(void){

    test_2();

    return 0;
}




