#include "shell.h"
#include "signal_handling.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
  HandledCommands id;
  const char *name;
} BuiltinEntry;

static const BuiltinEntry builtin_commands[] = {
    {CMD_CHANGE_DIR, "cd"},
    {CMD_EXIT, "exit"},
};

static const int signals_to_remove_in_child[] = {
    SIGTERM,
    SIGCHLD,
};

HandledCommands match_command(char *command) {
  int iterations_amount = sizeof(builtin_commands) / sizeof(BuiltinEntry);

  for (int i = 0; i < iterations_amount; i++) {
    if (command && strcmp(builtin_commands[i].name, command) == 0) {
      return builtin_commands[i].id;
    }
  }

  return CMD_UNKNOWN;
}

int handle_builtin(char **args) {
  HandledCommands command = match_command(args[0]);

  switch (command) {

  case CMD_CHANGE_DIR:
    if (args[1])
      chdir(args[1]);
    else
      perror("CD HAD NO ARGUMENTS \n");
    return 1;

  case CMD_EXIT:
    exit(0);

  case CMD_UNKNOWN:
    return 0; // not a built-in
  }

  return 0;
}

void execute_external(char **args) {
  int process = fork();

  if (process == 0) {
    remove_signal_handling((int *)signals_to_remove_in_child, 1);
    execvp(args[0], args);
    perror("Child process failed to execute\n");
    exit(1);
  }

  int status;
  waitpid(process, &status, 0);

  if (WIFEXITED(status)) {
    int code = WEXITSTATUS(status);
    if (code != 0) {
      printf("Child process exited with status: %d\n", code);
    }
  } else if (WIFSIGNALED(status)) {
    printf("Child process terminated with a signal: %d", WTERMSIG(status));
  }
}
