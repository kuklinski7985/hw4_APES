/**
* @file ipc_shrmem.c
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
#include <sys/mman.h>
#include <semaphore.h>

#include "ipc_shrmem.h"

extern sem_t sema_lock;

pthread_t thread1;
pthread_t thread2;
pthread_attr_t attr;

extern void * shrmem_addr1;
extern void * shrmem_addr2;

//writer first, then read
void *IPC_1(void *param)
{
  sem_wait(&sema_lock);
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

  printf("oringal sent: %s, USRLED: %d\n", (info->message),(info->USRLED));
  memcpy(shrmem_addr1, info, (sizeof(datashare)));
  sem_post(&sema_lock);

  sleep(1);
  datashare *infoback;
  infoback = (datashare*)malloc(sizeof(datashare));
  if(info ==NULL)
    {
      printf("IPC_pipe1 malloc failed\n");
      exit(1);
    }
  sem_wait(&sema_lock);
  infoback = shrmem_addr2;
  printf("received from 2: %s, USRLED: %d\n", (infoback->message),
  	 (infoback->USRLED));

  sem_post(&sema_lock);
  printf("leaving thread1\n");
  
}

//reader first, then write
void *IPC_2(void *param)
{
  sem_wait(&sema_lock);
  printf("entering thread2\n");
  datashare *info;
  datashare *infoback;

  info = (datashare*)malloc(sizeof(datashare));
  if(info ==NULL)
    {
      printf("IPC_pipe1 malloc failed\n");
      exit(1);
    }

  infoback = (datashare*)malloc(sizeof(datashare));
  if(info ==NULL)
    {
      printf("IPC_pipe1 malloc failed\n");
      exit(1);
    }
  info = shrmem_addr1;

  
  printf("received from 1: %s, USRLED: %d\n", (info->message),(info->USRLED));

  infoback->message = "Message received, Cartman";
  infoback->USRLED = 0;

  printf("new string sent: %s, USRLED: %d\n", (infoback->message),(infoback->USRLED));
  memcpy(shrmem_addr2, infoback, (sizeof(datashare)));


  sem_post(&sema_lock);
  printf("leaving thread2\n"); 
}

int run_shrmem()
{
  sem_init(&sema_lock,0,1);
  int shrmemsize = 64;
  /* shrmem_addr1 = mmap(NULL, shrmemsize, (PROT_READ | PROT_WRITE),
			    (MAP_ANONYMOUS | MAP_SHARED),0,0);
  
  shrmem_addr2 = mmap(NULL, shrmemsize, (PROT_READ | PROT_WRITE),
			    (MAP_ANONYMOUS | MAP_SHARED),0,0);
  */
  pthread_attr_init(&attr);
  int checking;


  checking = pthread_create(&thread1, &attr, IPC_1, NULL);
  if(checking)
    {
      fprintf(stderr, "error creating thread1");
      return -1;
    }
  
  checking = pthread_create(&thread2, &attr, IPC_2, NULL);
  if(checking)
    {
      fprintf(stderr, "error creating thread2");
      return -1;
    }


  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  sem_destroy(&sema_lock);
}


