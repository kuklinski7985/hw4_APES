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
#include "ipc_pipefxns.h"

extern pthread_t thread1;
extern pthread_t thread2;
extern pthread_attr_t attr;
extern int socket_handle;


int run_socket()
{

  /***********************creating socket**************************/

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

  if(connect(socket_handle, (struct sockaddr*)&socket_addr, sizeof(socket_addr))<0)
    {
      printf("did not connect\n");
      return -1;
    }

  printf("socket connected\n");
  if(bind(socket_handle,(struct sockaddr*)&socket_handle,sizeof(socket_handle))<0)
    {
      printf("bind failed\n");
      return -1;
    }
  printf("bind complete\n");
  

  /**********************************creating 2 threads**************************/
  pthread_attr_init(&attr);
  int checking;

  int * input_type;
  *(input_type) = 2;    //2 is for sockets
  
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
