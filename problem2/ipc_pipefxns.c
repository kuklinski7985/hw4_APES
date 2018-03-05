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

extern int pipe1[2], pipe2[2];
extern pthread_t thread1;
extern pthread_t thread2;
extern pthread_attr_t attr;
extern int socket_handle;

//writer first, then read
//client first(requesting data), the server giving data
void *IPC_1(void *param)
{
  int piperead;
  printf("entering thread1\n");

  int inputparam = *((int *)param);
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


  switch(inputparam){
    case 1:
      piperead = write(pipe1[1],info, sizeof(datashare));
      break;
    case 2:
      piperead = write(socket_handle,info,sizeof(datashare));
      break;
    default:
      printf("inputparam not valid\n");
  }

  datashare *infoback;
  infoback = (datashare*)malloc(sizeof(datashare));
  piperead = read(pipe2[0],infoback,MAXLENGTH);
  printf("string returned: %s, USRLED: %d\n", (infoback->message),
  	 (infoback->USRLED));

  close(socket_handle);
  printf("leaving thread1\n");
  
}

//reader first, then write
//socket server first(data requested out), then client
void *IPC_2(void *param)
{
  FILE * usrled4_handle = NULL;
  char * usrled4_brightness = "/sys/devices/platform/leds/leds/beaglebone:green:usr3/brightness";
  printf("entering thread2\n");
  datashare *info;
  info = (datashare*)malloc(sizeof(datashare));
  int piperead, socketaccept;
  int inputparam = *((int *)param);
  
  //piperead = read(pipe1[0],info,MAXLENGTH);

  switch(inputparam){
  case 1:
    piperead = read(pipe1[0],info,MAXLENGTH);
    break;
  case 2:
    listen(socket_handle,1);    //set up as server listening for requests
    
    piperead = read(socket_handle,info,sizeof(datashare));
    break;
  default:
    printf("inputparam not valid\n");
  }

  
  printf("string orginal: %s, USRLED: %d\n", (info->message),(info->USRLED));

  info->message = "Message received, Cartman";

  if((info->USRLED == 1) && (usrled4_handle = fopen(usrled4_brightness, "r+")))
    {
      printf("turning on LED\n\n");
      fwrite("1", sizeof(char),1,usrled4_handle);
    }
      
      
  info->USRLED = 0;
  

  //printf("passed in: %d\n", inputparam);
  switch(inputparam){
    case 1:
      piperead = write(pipe2[1],info, sizeof(datashare));
      break;
    case 2:
      piperead = write(socket_handle,info,sizeof(datashare));
      break;
    default:
      printf("inputparam not valid\n");
  }

  close(socket_handle);
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
  int * input_type;
  *(input_type) = 1;   //1 is for pipe

  checking = pthread_create(&thread2, &attr, IPC_2, input_type);
  if(checking)
    {
      fprintf(stderr, "error creating thread2");
      return -1;
    }

  checking = pthread_create(&thread1, &attr, IPC_1, input_type);
  if(checking)
    {
      fprintf(stderr, "error creating thread1");
      return -1;
    }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
}


