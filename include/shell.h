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
 */
int handle_builtin(char **args);
void execute_external(char **args);

#endif
