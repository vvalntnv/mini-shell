#include "debug.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void print_character(void *c) {
  char val = *(char *)c;
  printf("%c", val);
}
void print_string(void *s) {
  char *val = (char *)s;
  printf("%s", val);
}
void print_number(void *i) {
  int val = *(int *)i;
  printf("%d", val);
}

static inline int check_for_delimiter(DataType dt, void *v) {
  char *string = NULL;
  void *pointer = NULL;

  switch (dt) {
  case STRING:
    string = (char *)v;
    
    if (v == NULL || v == '\0') {
      return 0;
    };

    return 1;
  case POINTERS:
    pointers = (void *)v;
    return 1;
  default:
    return 0;
  }
}

void print_data(DataType data_type, void *data, size_t *array_size) {
  if (!data) {
    fprintf(stderr, "no data passed");
  }

  void (*print_function)(void *);

  switch (data_type) {
  case CHAR:
    print_function = print_string;
    break;
  case INTEGER:
    print_function = print_number;
    break;
  case STRING:
    print_function = print_string;
    break;
  default:
    fprintf(stderr, "something bad happened");
    exit(0);
  }

  if (array_size) {
    for (size_t i = 0; i < *array_size; i++) {
      printf("Index: %zu", i);
      print_function(&data[i]);
      printf("\n");
    }
  } else {
    size_t i = 0;

    if (data_type != STRING && data_type != POINTERS) {
      fprintf(stderr, "specify size! arrays without size are considered arrays "
                      "of pointers");
    }

    while (1) {
      void *current_data = &data[i];
      if (check_for_delimiter(data_type)) {
        break;
      }
      print_function(&data[i]);

      i++;
    }
  }
}
