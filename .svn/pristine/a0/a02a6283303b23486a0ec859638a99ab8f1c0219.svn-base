#include "editor.h"
#include "document.h"
#include "format.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This is the will be the entry point to your text editor.
*/
int main(int argc, char *argv[]) {
  // Checking to see if the editor is being used correctly.
  if (argc != 2) {
    print_usage_error();
    return 1;
  }
  // Setting up a docment based on the file named 'filename'.
  char *filename = get_filename(argc, argv);
  Document *document = Document_create_from_file(filename);

  // Buffer for the command and length of said buffer
  char *command = NULL;
  size_t len = 0;

  // This while loop will keep reading from stdin one line at a time
  // until the user enters 'q' (the quit command).
  int done = 0;
  while (!done) {
    getline(&command, &len, stdin);

    int command_type;

    // remove newline from the command
    char *nl = strchr(command, '\n');
    if (nl)
      *nl = 0;

    int stringlen = strlen(command);

    // if 'q' or 's' is first character only 'q' or 's' is allowed as valid
    // command
    if ((command[0] == 'q' || command[0] == 's') && stringlen != 1) {
      command_type = -1;
    } else {
      command_type = command[0];
    }

    switch (command_type) {
    case 'p':
      handle_display_command(document, command);
      break;
    case 'w':
      handle_write_command(document, command);
      break;
    case 'a':
      handle_append_command(document, command);
      break;
    case 'd':
      handle_delete_command(document, command);
      break;
    case '/':
      handle_search_command(document, command);
      break;
    case 's':
      if (strlen(command) == 1) {
        handle_save_command(document, filename);
      } else {
        invalid_command(command);
      }
      break;
    case 'q':
      done = 1;
      Document_destroy(document);
      break;
    default:
      invalid_command(command);
      break;
    }
  }

  // Need to free the buffer that we created.
  if (command) {
    free(command);
  }
}
