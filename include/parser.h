#ifndef PARSER_H
#define PARSER_H

/*
 * The user has to manually clean up the return
 * because the return type is allocated in the heap
 */
char **parse_input(char *input);

#endif
