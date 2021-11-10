#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef enum{
    oke, 
    fail
} status_t;

void demo0 (void);
void demo1(void);
void demo2( void );
void str_cpy(char *dest, char *source, uint16_t n);
status_t parse_str(void);


int main(void){
    parse_str();
    return 0;
}


void demo0 (void)
{
    char str[11u]= "123456789";
    printf("The string len: %d, size str: %d\n", strlen(str), sizeof(str));
}

void demo1(void)
{
    static char m,n;
    printf("nhap 2 size of string \n");
    scanf("%d%d", &m, &n);

    char len_1 = m-1, len_2= n-1, add= 0;
    char str[m], sstr[n], temp;
    while(len_1 >= 0){
        scanf("%d",(str+add));
        add++;
        len_1--;
    }

    printf("the str: %s, %d\n", str, *str);
    //printf("The result: %d\n", strcmp)
}

void demo2( void )
{
    char str[10u]= "helloABC", sstr[20u] = "helloAbc";
    printf("The compare: %d\n", strncmp(str, sstr, 7u));
}

void str_cpy(char *dest, char *source, uint16_t n)
{
    for( uint16_t i= 0; i< n; i++){
        *(dest+i) = *(source+i);
    }
}

status_t parse_str(void){
    char buff[10u];
    int age;
    //char *str = "{\"name\":\"hoa\", \"age\":\"23\", \"str\":}"buff"\r\n";
    char str[20];
    sprintf(str, "{\"%s\":\"%d\"}", "hoa", 23);
    printf("%s\n", str);
    sscanf(str,"{\"%s\":\"%d\"}", buff, &age);
    printf("the result-> name-> %s, age-> %d\n", buff, age);
    return oke;
}


/*
int day, year;
   char weekday[20], month[20], dtm[100];

   strcpy( dtm, "Tuesday July 12 2016" );
   sscanf( dtm, "%s %s %d  %d", weekday, month, &day, &year );

   printf("%s %d, %d = %s\n", month, day, year, weekday );
*/