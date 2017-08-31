/**
 * Machine Problem: Shell
 * CS 241 - Spring 2016
 */
#ifndef __CS241_LOG_H_
#define __CS241_LOG_H_
#include "vector.h"

typedef struct {
  Vector *vector;
} Log;

/* Creates and returns a new Log structure. */
Log *Log_create();

/**
 * Creates and returns a new Log structure
 * Open the file whose name is specified by 'filename'
 * Read all commands from the file and put them into the Log structure created.
 * If the file can not be opened or read for whatever reason, it returns an
 * empty log.
 */
Log *Log_create_from_file(const char *filename);

/**
 * Returns the number of commands in the Log structure.
 */
size_t Log_size(Log *log);

/**
 * Open the file whose name is specified by 'filename'
 * Write all the commands in the Log structure to the file opened.
**/

int Log_save(Log *log, const char *filename);

/**
 * Frees all the memory used by Log structure.
 */
void Log_destroy(Log *log);

/**
 * Appends a command to the Log structure at the end.
 */
void Log_add_command(Log *log, const char *command);

/**
 * Gets the command at the according index of the Log structure.
 */
const char *Log_get_command(Log *log, size_t index);

#endif
