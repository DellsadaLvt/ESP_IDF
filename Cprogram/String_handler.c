#include <stdio.h>
#include <string.h>


int main( void ){


    char str[100]= "this is the test @@@@@ 12345678";
    const char key[6]= "@@@@@";
    char *token;

    char ssid[30], pass[30];
    token = strtok(str, key);
    sprintf(ssid, "%s", token);
    token = strtok(NULL, key);
    sprintf(pass, "%s", token);
    printf("The ssid: %s\n", ssid);
    printf("The pass: %s\n", pass);

    return 0;
}


