/**
* @file ipc_socketfxns.c
* @brief  functions definitions for homework4, IPC control
* @author Andrew Kuklinski
* @date 03/06/2018
**/


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ipc_socketfxns.h"

pthread_t thread1;
pthread_t thread2;
pthread_attr_t attr;
int socket_handle;


int run_socket()
{


  if(connect(socket_handle, (struct sockaddr*)&socket_addr, sizeof(socket_addr))<0)
    {
      printf("did not connect\n");
      return -1;
    }

  printf("socket connected\n");

  

  /**********************************creating 2 threads**************************/
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

//client first(requesting data), the server giving data
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
  piperead = write(socket_handle,info,sizeof(datashare));
 

  datashare *infoback;
  infoback = (datashare*)malloc(sizeof(datashare));

  printf("string returned: %s, USRLED: %d\n", (infoback->message),
  	 (infoback->USRLED));

  close(socket_handle);
  printf("leaving thread1\n");
  
}

//socket server first(data requested out), then client
void *IPC_2(void *param)
{
  printf("entering thread2\n");

  int client_accept;
  struct sockaddr_in socket_addr;
  socket_handle = socket(AF_INET, SOCK_STREAM,0);
  if(socket_handle == -1)
    {
      printf("socket creation failed\n");
      return -1;
    }
  printf("socket created\n");

  socket_addr.sin_family = AF_INET;
  socket_addr.sin_addr.s_addr = INADDR_ANY;
  socket_addr.sin_port = htons(80);

  if(bind(socket_handle,(struct sockaddr*)&socket_handle,sizeof(socket_handle))<0)
    {
      printf("bind failed\n");
      return -1;
    }
  printf("bind complete\n");

  client_accept = accept(socket_handle, (struct sockaddr *) &socket_addr, (socklen_t*)(struct sockaddr_in));

  
  
  datashare *info;
  info = (datashare*)malloc(sizeof(datashare));
  int piperead, socketaccept;
  
  listen(socket_handle,1);    //set up as server listening for requests
  piperead = read(socket_handle,info,sizeof(datashare));
  
  printf("string orginal: %s, USRLED: %d\n", (info->message),(info->USRLED));

  info->message = "Message received, Cartman";
  
  info->USRLED = 0;
  piperead = write(socket_handle,info,sizeof(datashare));

  close(socket_handle);
  printf("leaving thread2\n"); 
}
