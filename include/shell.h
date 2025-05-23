#ifndef SHELL_H
#define SHELL_H

typedef enum {
  CMD_CHANGE_DIR,
  CMD_EXIT,
  CMD_UNKNOWN,
} HandledCommands;

/**
 * Will return 0 if the args cannot be handled, and 1 if the args include
 * a handled command
 *
 * @param args A NULL-terminated array of strings.
 * @return 1 if the command was handled, 0 otherwise.
 */
int handle_builtin(char **args);

/**
 * Executes an external command.
 *
 * @param args A NULL-terminated array of strings containing the command and its
 * arguments.
 */
void execute_external(char **args);

void sigint_handler(int signo);

#endif
