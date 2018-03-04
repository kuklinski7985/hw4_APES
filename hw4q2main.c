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
#include "ipcfxns.h"

pthread_t thread1;
pthread_t thread2;
pthread_attr_t attr;

int pipe1[2], pipe2[2];


int main(int argc, char *argv[])
{
  if(argc == 1)
    {
      printf("Please enter an IPC option\n");
      printf("1:pipe, 2:socket, 3:shared mem, 4: queue\n");
      printf("Please run again\n");
      exit(1);
    }

  if(pipe(pipe1))
    {
      fprintf(stderr, "pipe1 creation failed\n");
      return -1;
    }

  if(pipe(pipe2))
    {
      fprintf(stderr, "pipe2 creation failed\n");
      return -1;
    }

  pthread_attr_init(&attr);
  int checking;

  checking = pthread_create(&thread2, &attr, IPC_pipe2, NULL);
  if(checking)
    {
      fprintf(stderr, "error creating thread2");
      return -1;
    }

  checking = pthread_create(&thread1, &attr, IPC_pipe1, NULL);
  if(checking)
    {
      fprintf(stderr, "error creating thread1");
      return -1;
    }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  return 0;

  
}

