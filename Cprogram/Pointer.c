#include <stdio.h>



int main( void ){
    int a = 320;
    
    /* test 1 */
    char *ptr;
    ptr = (char *)&a;
    printf("the value test is: %d\n", *ptr);
    *(ptr+1) = 2;
    printf("the value test is: %d\n", *(ptr+1));
    printf("the value test is: %d\n", a);

    /* test 2 */
    int *ptr_1;
    ptr_1 = (char *)&a;
    printf("the value test is: %d\n", *ptr_1);

    return 0;
}



