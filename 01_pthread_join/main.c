/*
 * Systems Group, D-INFK, ETH Zurich
 *
 * Author  : A.Ramdas
 * Date    : 2024-01-24
 * Project : Multithreading
 *
 */

#include<stdio.h>
#include<errno.h> //errno, perror
#include<stdlib.h> //calloc, EXIT_SUCCESS/FAILURE
#include<pthread.h> 

/*
 * The child thread can return pointer to data to the 
 * parent thread. This data should reside in the heap 
 * and not on the stack of the child thread.
 * 
 * Any variable declared within the thread go into 
 * the thread's stack and is thread local variable.
 * thread local varaibles cannot be accessed by the parent.
 * 
 * pthread_join(thread, void **ret) will make parent thread 
 * wait for child and get its value.
 */

// handle error with error number
#define handle_error_en(en, msg) \
  do { errno = en; perror(msg); exit(EXIT_FAILURE); } while(0)

typedef struct my_arg_ {
  int a;
  int b;
} my_arg_t;

void *callback_fn(void *){
  // create data in the heap and return it
  // void * everywhere because both arguments and return
  // values can be casted.
  my_arg_t *m1 = (my_arg_t*) calloc(1, sizeof(my_arg_t));
  m1->a = 10;
  m1->b = 20;
  return((void*)m1);
				    
}

int main(int argc, char *argv[]){

  pthread_t myThread;

  // pointer to location of return data in heap
  my_arg_t *thrRetVal;
  
  // not passing any input arguments to the thread 
  int s = pthread_create(&myThread, NULL, callback_fn, NULL);
  if(s != 0){
    handle_error_en(s, "pthread_create");
  }

  // wait for pthread to join
  // pass the address of the pointer so that
  // the address in the heap will be copied into this location.
  s = pthread_join(myThread, (void **)&thrRetVal);
  if(s != 0){
    handle_error_en(s, "pthread_join");
  }
  
  printf("Returned value: %d, %d\n", thrRetVal->a, thrRetVal->b);

  return EXIT_SUCCESS; 
}

