#ifndef SIGNAL_HANDLING_H
#define SIGNAL_HANDLING_H

#include <stddef.h>
void setup_signal_handling();
void remove_signal_handling(int *signals_to_remove, size_t size);

#endif
