/*Homework #4, question 2.
 *creating a pipe, socket, shared memory, and queue*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ipc_pipefxns.h"
#include "ipc_socketfxns.h"

pthread_t thread1;
pthread_t thread2;
pthread_attr_t attr;

int pipe1[2], pipe2[2];

int socket_handle;

int main(int argc, char *argv[])
{
  if(argc == 1)
    {
      printf("Please enter an IPC option\n");
      printf("1:pipe, 2:socket, 3:shared mem, 4: queue\n");
      printf("Please run again\n");
      exit(1);
    }

  int things;
  things = atoi(argv[1]);
  switch (things){

  case 1:
    printf("running pipe\n");
    run_pipe();
    break;
  case 2:
    printf("running socket\n");
    run_socket();
    break;
  case 3:
    printf("running shared memory\n");
    break;
  case 4:
    printf("running queue\n");
  default:
    printf("Not valid\n");
  }
    

  return 0;

  
}

