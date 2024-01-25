/*
 * Systems Group, D-INFK, ETH Zurich
 *
 * Author  : A.Ramdas
 * Date    : 2024-01-24
 * Project : Multithreading
 *
 */


#include<stdio.h>
#include<errno.h>
#include<stdlib.h> // calloc
#include<pthread.h>

/*
Using pthread create to launch a child thread.
Very simple example where pthread takes default attributes and returns nothing

Pthread create needs the following
a. A pthread variable
b. The function to be executed by the thread (Callback function)
c. arguments to be passed on to the thread.

Remember
1. The arguments passed to the thread should be on the heap or static variable

 */


#define handle_error_en(en, msg) \
  do {errno = en; perror(msg); exit(EXIT_FAILURE); } while(0)

typedef struct {
  int a;
  int b;
} my_arg_t;


void* my_thread_callable(void *arg){
  // callable by the thread
  // just prints the input argument and returns NULL pointer
  // the argument passed is void * so it can be casted as necessary.
  my_arg_t *m1 = (my_arg_t *) arg;
  printf("Thread: %d, %d\n", m1->a, m1->b);
  return NULL;
}


int main(int argc, char *argv[]){

  pthread_t my_thread;

  // Allocate variable in heap so it is accessible by thread
  my_arg_t *tvar = (my_arg_t *)calloc(1, sizeof(my_arg_t));
  tvar->a = 10;
  tvar->b = 20;

  // call pthread create
  int s = pthread_create(&my_thread, NULL, my_thread_callable, (void *) tvar);
  if(s != 0){
    handle_error_en(s, "pthread_create");
  }

  // creator thread waits for child to join
  // No value returned by child thread
  // also not checking result of pthread_join 
  (void) pthread_join(my_thread, NULL);

  printf("Done\n");
  
  return 0; // good
}

