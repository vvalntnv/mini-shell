#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <stddef.h>
enum DataType {
  CHAR,
  INTEGER,
  STRING,
  STRUCT,
  POINTERS,
};

typedef enum DataType DataType;

void print_data(DataType data_type, void *data, size_t *array_size);

#define LOG_ARRAY(array, array_size)                                           \
  print_data(array, (void *)array, array_size);

#endif
