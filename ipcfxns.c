/**
* @file ipcfxns.c
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

#include "ipcfxns.h"

extern int pipe1[2], pipe2[2];

//writer first, then read
void *IPC_pipe1(void *param)
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
  
  //int piperead;

  piperead = write(pipe1[1],info, sizeof(datashare));

  datashare *infoback;
  infoback = (datashare*)malloc(sizeof(datashare));
  piperead = read(pipe2[0],infoback,MAXLENGTH);
  printf("string returned: %s, USRLED: %d\n", (infoback->message),
  	 (infoback->USRLED));
  
  printf("leaving thread1\n");
  
}

//reader first, then write
void *IPC_pipe2(void *param)
{
  FILE * usrled4_handle = NULL;
  char * usrled4_brightness = "/sys/devices/platform/leds/leds/beaglebone:green:usr3/brightness";
  printf("entering thread2\n");
  datashare *info;
  info = (datashare*)malloc(sizeof(datashare));
  int piperead;
  piperead = read(pipe1[0],info,MAXLENGTH);

  
  printf("string orginal: %s, USRLED: %d\n", (info->message),(info->USRLED));

  info->message = "Message received, Cartman";

  if((info->USRLED == 1) && (usrled4_handle = fopen(usrled4_brightness, "r+")))
    {
      printf("turning on LED\n\n");
      fwrite("1", sizeof(char),1,usrled4_handle);
    }
      
      
  info->USRLED = 0;
  piperead = write(pipe2[1],info, sizeof(datashare));

  printf("leaving thread2\n"); 
}
