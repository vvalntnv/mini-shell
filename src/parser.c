#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ARGS 100

char **parse_input(char *input) {
  char **args = malloc(sizeof(char *) * 100);
  int i = 0;
  char *token = strtok(input, " ");

  while (token != NULL) {
    if (i >= MAX_ARGS - 1) {
      perror("Max args have been reached");
      return NULL;
    }
    args[i++] = token;
    token = strtok(NULL, " ");
  }

  args[i] = NULL;

  return args;
}
