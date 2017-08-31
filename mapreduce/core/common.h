/**
 * MapReduce
 * CS 241 - Spring 2016
 */

#ifndef COMMON_H_MR
#define COMMON_H_MR

#include <sys/types.h>

/**
 * Returns a hash of the given string.
 */
unsigned int hashKey(char *key);

/**
 * Add to the array of additional file descriptors.
 */
void descriptors_add(int fd);

/**
 * Close all the file descriptors passed to descriptors_add();
 */
void descriptors_closeall();

/**
 * Deallocate the memory used to start the file descriptor list.
 */
void descriptors_destroy();

/**
 * splits a line by it's key and value.
 *
 * This function does not allocate memory. It's two outputs are both pointers
 * into the (now modified) line which is passed in.
 *
 * @param line - a newline terminated line, will be modified!
 * @param key - output
 * @param value - output
 *
 * @return "boolean" representing success or failure
 */
int split_key_value(char *line, char **key, char **value);

#endif
