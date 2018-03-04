/**
* @file ipcfxns.h
* @brief protoytpe functions for homework4, IPC control
* @author Andrew Kuklinski
* @date 03/06/2018
**/


#ifndef ipcfxns_h_
#define ipcfxns_h_

typedef struct datashare{
  int USRLED;
  char * message;
  int max_length;
}datashare;

#define MAXLENGTH   32

void *IPC_pipe1(void *param);

void *IPC_pipe2(void *param);


#endif /*__ipcfxns_h_*/
