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
#include <semaphore.h>
#include "ipc_shrmem.h"

pthread_t thread1;
pthread_t thread2;
pthread_attr_t attr;

sem_t sema_lock;

void * shrmem_addr1;
void * shrmem_addr2;

int main(int argc, char *argv[])
{
    int shrmemsize = 64;

    shrmem_addr1 = mmap(NULL, shrmemsize, (PROT_READ | PROT_WRITE),
			    (MAP_ANONYMOUS | MAP_SHARED),0,0);
  
  shrmem_addr2 = mmap(NULL, shrmemsize, (PROT_READ | PROT_WRITE),
			    (MAP_ANONYMOUS | MAP_SHARED),0,0);

  run_shrmem();

  return 0;
}

//dont forget to use make build to compile, run use ./main.elf
