#include "shell.h"
#include "jobs.h"
#include "signal_handling.h"
#include <signal.h>
#include <stddef.h>
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
    {CMD_LS_JOBS, "jobs"},
};

static const int signals_to_remove_in_child[] = {SIGTERM, SIGCHLD, SIGINT,
                                                 SIGTSTP};

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
    break;

  case CMD_EXIT:
    exit(0);

  case CMD_LS_JOBS:
    list_jobs();
    break;

  case CMD_UNKNOWN:
    return 0; // not a built-in
  }

  Job job = {0, 0, args[0], JOB_RUNNING};
  register_job(&job);

  return 1;
}

/**
 * @brief Must be from a NULL terminated Array
 */
size_t get_array_count(void *const *args) {
  if (args == NULL || args[0] == NULL) {
    return 0;
  }

  size_t count = 1;
  while (args[count] != NULL) {
    count++;
  }
  return count;
}

void execute_external(char **args) {
  int is_background = 0;
  size_t last_element_index = get_array_count((void *)args) - 1;

  // Sanitize whitespace someday
  if (strcmp(args[last_element_index], "&") == 0) {
    is_background = 1;
    args[last_element_index] = NULL;
  }

  int process_id = fork();
  if (process_id < 0) {
    perror("FORK FAILED :(");
  }

  if (process_id == 0) {
    // Create a new process group with the same id
    setpgid(0, 0);
    remove_signal_handling((int *)signals_to_remove_in_child, 1);
    execvp(args[0], args);
    perror("Child process failed to execute\n");
    exit(1);
  }

  if (is_background) {
    printf("Process running in the background, with id: %d :)\n", process_id);
    return;
  }

  int status;
  setpgid(process_id, process_id);

  tcsetpgrp(STDIN_FILENO, process_id);
  waitpid(process_id, &status, 0);
  tcsetpgrp(STDIN_FILENO, getpid());

  if (WIFEXITED(status)) {
    int code = WEXITSTATUS(status);
    if (code != 0) {
      printf("Child process exited with status: %d\n", code);
    }
  } else if (WIFSIGNALED(status)) {
    printf("Child process terminated with a signal: %d\n", WTERMSIG(status));
  }
}
