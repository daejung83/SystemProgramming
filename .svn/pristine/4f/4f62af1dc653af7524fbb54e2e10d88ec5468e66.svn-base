#include "editor.h"
#include "document.h"
#include "format.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * You can programatically test your text editor.
*/
int main() {
  // Setting up a docment based on the file named 'filename'.
  char *filename = "test.txt";
  Document *document = Document_create_from_file(filename);

  handle_write_command(document, "w 1 hello");
  handle_display_command(document, "p");
}
