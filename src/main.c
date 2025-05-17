#include "parser.h"
#include "shell.h"
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
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
