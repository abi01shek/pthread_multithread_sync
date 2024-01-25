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
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h> // sleep()

/*
 * If main thread terminates by reaching end of main before 
 * the child threads complete, all child threads will be killed
 * because the entire process gets terminated. The zombile child 
 * threads would be cleaned up by the OS. 
 *
 * But if the main thread exits using pthread_exit() then the 
 * process itself is not killed and the child thread continue to execute 
 * till they complete.
 */


#define handle_errno_en(en, msg) \
  do {errno = en; perror(msg); exit(EXIT_FAILURE);} while(0)

void *callbackFn(void *arg){
  // sleep 100 seconds and terminate
  for(int i = 0; i <10; i++){
    printf("i = %d\n", i);
    sleep(1);
  }
  return NULL; 
}


int main(int argc, char *argv[]){

  pthread_t threads[10];

  for(int i = 0; i<10; i++){
    int s = pthread_create(&threads[i], NULL, callbackFn, NULL);
    if(s != 0){
      handle_errno_en(s, "pthread_create");
    }
  }

  // if main thread exits with just return statement
  // the process will be killed along with all child threads
  // if main thread exits with pthread_exit then process
  // is not killed.

  //pthread_exit(NULL);
  
  return 0; // good
}

