/*Homework #4, question 2.
 *creating a pipe, socket, shared memory, and queue*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct datashare{
  int USRLED;
  char * message;
  int max_length;
}datashare;

#define MAXLENGTH   32

//datashare *info;

pthread_t thread1;
pthread_t thread2;
pthread_attr_t attr;

int pipe1[2], pipe2[2];

void *IPC_pipe1(void *param);
void *IPC_pipe2(void *param);

int main(int argc, char *argv[])
{
  if(argc == 1)
    {
      printf("Please enter an IPC option\n");
      printf("1:pipe, 2:socket, 3:shared mem, 4: queue\n");
      printf("Please run again\n");
      exit(1);
    }

  /*info = (datashare*)malloc(sizeof(datashare));
  info->message = "beefcake!111111112345";
  info->max_length = MAXLENGTH;
  if((strlen(info->message)) > (info->max_length))
    {
      printf("input string longer than %d characters!\n",(info->max_length));
	exit(1);
    }
    info->USRLED = 1;*/
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
  
  /*will wait until child process is complete
   *since thread1 is already complete with previous join
   *waiting on thread2
   */
  //wait(NULL); 
  pthread_join(thread2, NULL);
  return 0;

  
}

//writer first, then read
void *IPC_pipe1(void *param)
{
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
  
  int piperead;

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
  printf("entering thread2\n");
  datashare *info;
  info = (datashare*)malloc(sizeof(datashare));
  int piperead;
  piperead = read(pipe1[0],info,MAXLENGTH);

  printf("string orginal: %s, USRLED: %d\n", (info->message),(info->USRLED));

  info->message = "Message received, Cartman";
  info->USRLED = 0;
  piperead = write(pipe2[1],info, sizeof(datashare));

  printf("leaving thread2\n");
  
  
}
