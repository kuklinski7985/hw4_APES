/*Homework #4, question 2.
 *creating a pipe, socket, shared memory, and queue*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <semaphore.h>
#include "ipc_queue_ex.h"

pthread_t thread1;
pthread_t thread2;
pthread_attr_t attr;

sem_t sema_lock;

key_t key;
int queueID;

int main(int argc, char *argv[])
{

  run_queue_ex();
  return 0;
}

//dont forget to use make build to compile, run use ./main.elf
