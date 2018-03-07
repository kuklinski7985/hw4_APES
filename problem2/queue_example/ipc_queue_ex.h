/**
* @file ipc_pipefxns.h
* @brief protoytpe functions for homework4, IPC control
* @author Andrew Kuklinski
* @date 03/06/2018
**/


#ifndef ipc_shrmem_h_
#define ipc_shrmem_h_

typedef struct datashare{
  int USRLED;
  char * message;
  int max_length;
  long mesg_type;  //used for message ID in message queue
}datashare;

#define MAXLENGTH   32

void *IPC_1(void *param);

void *IPC_2(void *param);

int run_queue_ex();



#endif /*__ipc_shrmem_h_*/
