#include "signal_handling.h"
#include <readline/readline.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void redisplay_input() {
  rl_replace_line("", 0);
  rl_on_new_line();
  rl_redisplay();
}

void sigint_handler(int signo) {
  printf("Someone tried to kill mee.... ;(\n");
  printf("Received a signal with number: %d\n", signo);
  redisplay_input();
}

void sigchld_handler(int signal __attribute__((unused))) {
  int status;
  pid_t pid;

  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    printf("[SIGCHLD] Received for a child %d\n", pid);
  }
}

void setup_signal_handling() {
  signal(SIGINT, sigint_handler);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGCHLD, sigchld_handler);
}

void remove_signal_handling(int *signals, size_t size) {
  for (size_t i = 0; i < size; i++) {
    signal(signals[i], SIG_DFL);
  }
}
