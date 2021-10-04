#include <stdio.h>
#include <stdint.h>

// this is in practice two
typedef void (*func_callback_t)(void);
func_callback_t input_callback= NULL;

void Func( void );
void input_set_callback( void * cb );
void call_func( void *arg);
void str_input( uint32_t *data );


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

    /* ex_5 */
    call_func(Func);


    return 0;
}

void call_func( void *arg){
    void (*call_func_t)(void);
    call_func_t= arg;
    call_func_t();
}

void Func( void ){
    printf("Hello World!\n");
}

/* update from practice two */
void input_set_callback( void * cb ){
      input_callback= cb;
}







