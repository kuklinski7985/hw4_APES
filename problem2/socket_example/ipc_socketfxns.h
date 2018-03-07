/**
* @file ipc_socketfxns.h
* @brief protoytpe functions for homework4, IPC control
* @author Andrew Kuklinski
* @date 03/06/2018
**/


#ifndef ipc_socketfxns_h_
#define ipc_socketfxns_h_

typedef struct datashare{
  int USRLED;
  char * message;
  int max_length;
}datashare;

#define MAXLENGTH   32

void *IPC_1(void *param);

void *IPC_2(void *param);

int run_socket();


#endif /*__ipc_socketfxns_h_*/
