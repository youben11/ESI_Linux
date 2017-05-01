#ifndef DEF_SCRIPT_CTRL
#define DEF_SCRIPT_CTRL

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

typedef struct{
  int pid;
  int stdout_fd;
  int stderr_fd;
}ps_info;

ps_info script_ctrl(char *script,char **argv);

#endif