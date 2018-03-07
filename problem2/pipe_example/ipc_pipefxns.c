/**
* @file ipc_pipefxns.c
* @brief  functions definitions for homework4, IPC control
* @author Andrew Kuklinski
* @date 03/06/2018
**/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ipc_pipefxns.h"

int pipe1[2], pipe2[2];
pthread_t thread1;
pthread_t thread2;
pthread_attr_t attr;

//writer first, then read
void *IPC_1(void *param)
{
  int piperead;
  printf("entering thread1\n");

  datashare *info;
  info = (datashare*)malloc(sizeof(datashare));
  if(info ==NULL)
    {
      printf("IPC_pipe1 malloc failed\n");
      exit(1);
    }
  info->message = "beefcake! BEEFCAKE!!";
  info->max_length = MAXLENGTH;
  if((strlen(info->message)) > (info->max_length))
    {
      printf("input string longer than %d characters!\n",(info->max_length));
	exit(1);
    }
  info->USRLED = 1;
  piperead = write(pipe1[1],info, sizeof(datashare));

  datashare *infoback;
  infoback = (datashare*)malloc(sizeof(datashare));
  piperead = read(pipe2[0],infoback,MAXLENGTH);
  printf("string returned: %s, USRLED: %d\n", (infoback->message),
  	 (infoback->USRLED));

  printf("leaving thread1\n");
  
}

//reader first, then write
void *IPC_2(void *param)
{
  printf("entering thread2\n");
  datashare *info;
  info = (datashare*)malloc(sizeof(datashare));
  int piperead;

  piperead = read(pipe1[0],info,MAXLENGTH);
  
  printf("string orginal received: %s, USRLED: %d\n", (info->message),(info->USRLED));

  info->message = "Message received, Cartman";
  info->USRLED = 0;
  piperead = write(pipe2[1],info, sizeof(datashare));

  printf("leaving thread2\n"); 
}

int run_pipe()
{
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

  checking = pthread_create(&thread2, &attr, IPC_2, NULL);
  if(checking)
    {
      fprintf(stderr, "error creating thread2");
      return -1;
    }

  checking = pthread_create(&thread1, &attr, IPC_1, NULL);
  if(checking)
    {
      fprintf(stderr, "error creating thread1");
      return -1;
    }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
}


