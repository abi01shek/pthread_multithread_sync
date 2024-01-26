/*
 * Systems Group, D-INFK, ETH Zurich
 *
 * Author  : A.Ramdas
 * Date    : 2024-01-26
 * Project : Multithreading
 *
 */

#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<pthread.h>
#include<error.h>
#include<unistd.h>

#define handle_err_en(en, msg) \
  do { errno = en; perror(msg); exit(EXIT_FAILURE); } while(0)


void *my_thread_callback(void * arg){
  // calculates the square of the integer passed to it.
  // make sure the pointer returned is a pointer to the heap

  // copy value of arg from heap to local variable and free it.
  int val = * (int *) arg;
  free(arg);

  // allocate return value in the heap and not within this
  // functions stack so that the parent thread can see its contents.
  // make sure parent thread frees this variable after use. 
  int *val_squared = (int *) calloc(1, sizeof(int));
  if(val_squared == NULL){
    printf("Calloc failed\n");
    exit(EXIT_FAILURE);
  }
  // calculate square of value and store it in the heap. 
  *val_squared = val * val;
  printf("Child value: %d\n", val);

  sleep(val);
  
  // cast return value pointer to void * before returning
  return((void*)val_squared);
}



void my_thread_create(pthread_t *my_thread, int my_thread_arg){
  // Function creates a joinable thread and passes an argument to it.
  // does not return any value 

  // Create an attribute for the thread and mark it as joinable 
  pthread_attr_t attr;
  int s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  if(s != 0){
    handle_err_en(s, "pthread_attr_setdetach_state");
  }

  // allocate memory in heap to pass to the thread
  int *my_thread_arg_heap = (int *) calloc(1, sizeof(int));
  if(my_thread_arg_heap == NULL){
    printf("Calloc failed\n");
    exit(EXIT_FAILURE);
  }
  // copy local data over to the heap
  *my_thread_arg_heap = my_thread_arg;


  // Create a pthread
  s = pthread_create(my_thread,
		     &attr,
                     my_thread_callback,
		     (void *) my_thread_arg_heap
		     );
  if(s != 0){
    handle_err_en(s, "pthread_create");
  }
  return;
}


void my_thread_join(pthread_t my_thread){
  void *ret_val_heap;

  // join with child thread and pass the address of a pointer
  // to copy over the pointer to the data in the heap.
  // ret_val_heap's address is passed so the return value pointer's
  // address can be copied over here. 
  (void) pthread_join(my_thread, &ret_val_heap);

  // print the results and free the pointer
  printf("Value returned: %d\n", *  (int *)ret_val_heap);
  free(ret_val_heap);

  return;
}


int main(int argc, char *argv[]){
  pthread_t thread1, thread2;

  my_thread_create(&thread1, 8);
  my_thread_create(&thread2, 3);

  my_thread_join(thread1);
  my_thread_join(thread2);
  
  return 0; // good
}

