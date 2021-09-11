#include <stdio.h>

// this is in practice two
typedef void (*func_callback_t)(void);
func_callback_t input_callback= NULL;

void Func( void );
void input_set_callback( void * cb );

int main ( void ){
    /* practice one */
    void (*func)( void );
    func= &Func;
    func();

    /* practice two */
    input_callback= Func;
    input_callback();

    /* update from practice two */
    input_set_callback(Func); // Func is the call back function, it can be at another file.
    input_callback();


    return 0;
}

void Func( void ){
    printf("Hello World\n");
}


/* update from practice two */
void input_set_callback( void * cb ){
      input_callback= cb;
}



