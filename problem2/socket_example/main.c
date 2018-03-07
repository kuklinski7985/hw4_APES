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
#include "ipc_socketfxns.h"


int main(int argc, char *argv[])
{
  run_socket();

  return 0;
}

//dont forget to use make build to compile, run use ./main.elf
