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
#include<pthread.h>
#include<stdlib.h>

#define MAX_CHANCE 10

/*
 * A child thread terminates when the callback function 
 * terminates or if pthread_exit() is explicitly called
 * 
 * lets use pthread_exit in this case 
 * 
 * Create a child thread that continusously takes numbers 
 * as input from the user. if number is divisible by 5 then 
 * terminate the child and finish
 */


#define handle_errno_en(en, msg) \
  do { errno = en; perror(msg); exit(EXIT_FAILURE); } while(0)

void* thrCallback(void *arg){
  int a = 0;
  for(int i=0; i<MAX_CHANCE; i++){
    printf("Enter a number divisible by 5: ");
    scanf("%d", &a);
    if(a % 5 != 0){
      printf("You have %d tries, try again\n", (MAX_CHANCE-i-1));
    } else{
      printf("Correct! exiting thread\n");
      pthread_exit(NULL);
    }
  }
  printf("ok bye Thread terminating normally\n");
  return NULL;
}

int main(int argc, char *argv[]){

  pthread_t myThread;

  int s = pthread_create(&myThread, NULL, thrCallback, NULL);
  if(s != 0){
    handle_errno_en(s, "pthread_create");
  }

  (void) pthread_join(myThread, NULL);
  printf("done\n");
  
  return 0; // good
}

