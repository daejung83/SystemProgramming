#ifndef __CS241_FORMAT_H__
#define __CS241_FORMAT_H__
#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>

/**
 * This library will handle all the formatting
 * for your shell.
 *
 * Please use this to ensure your formatting
 * matches what the autograder expects.
 */

/**
 * Before the first prompt is printed, print shell owner.
 * @param netid Your netid!
 */
void print_shell_owner(const char *netid);

/**
 * Print the correct usage on invalid arguments.
 */
void print_usage();

/**
 * Print when the user specifies a non-existent script file.
 */
void print_script_file_error();

/**
 * Print the shell prompt.
 * @param directory Current directory
 * @param pid The current process id
 */
void print_prompt(const char *directory, pid_t pid);

/**
 * Print when the user specifies and out-of-bounds history command.
 */
void print_invalid_index();

/**
 * Print when the history search fails (no command has the prefix). 
 */
void print_no_history_match();

/**
 * Print when the user tries to enter a non-existant directory.
 * @param path The nonexistant directory
 */
void print_no_directory(const char *path);

/**
 * Print when a different process executes a command.
 * @param The current process id
 */
void print_command_executed(pid_t pid);

/**
 * Print when you fail to fork.
 */
void print_fork_failed();

/**
 * Print when you fail to exec.
 */
void print_exec_failed(const char* command);

/**
 * Print when you fail to wait.
 */
void print_wait_failed();

/**
 * Splits a copy of 'str' by a 'delim'.
 * Then writes the number of tokens into 'numtokens'.
 * These strings are allocated on the heap, so call free_args(char **args) when
 * you are done with them.
 * Learn more at:
 * https://www.quora.com/How-do-you-write-a-C-program-to-split-a-string-by-a-delimiter
 */
char **strsplit(const char *str, const char *delim, size_t *numtokens);

/**
 * Frees the memory returned by strsplit().
 */
void free_args(char **args);

/**
 * Returns the full path a file named 'filename' in the current directory.
 * The path is stored on the heap, so it is up the caller to free it.
 */
char *get_full_path(char *filename);

#endif
