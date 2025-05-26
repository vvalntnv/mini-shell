#include "parser.h"
#include "jobs.h"
#include "shell.h"
#include "signal_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_JOBS_COUNT 100

Job *jobs_map = NULL;
int jobs_count = 0;

int main() {
  pid_t shell_pgid = getpid();
  setpgid(shell_pgid, shell_pgid);
  tcsetpgrp(STDIN_FILENO, shell_pgid);

  setup_signal_handling();

  while (1) {
    char *input = readline("viki-shell> ");
    if (!input)
      break;

    if (input[0] == '\0')
      continue;

    char **args = parse_input(input);

    if (!args)
      continue;

    int code = handle_builtin(args);
    if (code == -1) {
      perror("THIS SHOULD HAVE NEVER HAPPENED :(");
      exit(1);
    }

    if (!code) {
      execute_external(args);
    }

    free(input);
    free(args);
  }
  return 0;
}
