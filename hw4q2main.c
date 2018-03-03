/*Homework #4, question 2.
 *creating a pipe, socket, shared memory, and queue*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

typedef struct datashare{
  int USRLED;
  char * message;
}datashare;

datashare *info;

pthread_t thread1;
pthread_t thread2;
pthread_attr_t attr;

int pipe1[2], pipe2[2];

void *IPC_pipe1(void *param);
void *IPC_pipe2(void *param);

int main(int argc, char *argv[])
{

  info = (datashare*)malloc(sizeof(datashare));
  info->message = "beefcake!1111111123456789012345xxx";
  info->USRLED = 1;
  if(pipe(pipe1))
    {
      fprintf(stderr, "pipe1 creation failed\n");
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

//writer
void *IPC_pipe1(void *param)
{
  printf("entering thread1\n");
  int piperead;
  piperead = write(pipe1[1],info->message, (strlen(info->message)+1));
  printf("leaving thread1\n");
  
}

//reader
void *IPC_pipe2(void *param)
{
  printf("entering thread2\n");
  char *input;
  input = (char*)malloc(sizeof(char)*32);
  int piperead;
  piperead = read(pipe1[0],input,(strlen(info->message)+1));
  printf("string: %s\n", input);
  printf("leaving thread2\n");
}
