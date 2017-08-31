#ifndef __CS241_DOCUMENT_H__
#define __CS241_DOCUMENT_H__
#include "vector.h"

typedef struct {
  Vector *vector;
} Document;

/** Allocate and return a new Document structure. */
Document *Document_create();

/**
 * Writes the content of 'document' to a file named 'filename'.
 *
 * For each line in 'document' write it to 'filename' with the format ("%s\n",
 * line), where line is a null terminated string.
 *
 * No entries in the document can be NULL. An empty line in the document should
 * be written as the string "\n"
 */
void Document_write_to_file(Document *document, const char *filename);

/**
 * Creates a document and loads with the information from
 * the file named 'filename'.
 *
 * Note if for any reason you can not open the file
 * then you should treat it as if it is an empty file
 * and move on.
 */
Document *Document_create_from_file(char *filename);

/**
 * Returns the number of lines in use in 'document'.
 */
size_t Document_size(Document *document);

/**
 * Frees all memory used to represent 'document'
 */
void Document_destroy(Document *document);

/**
 * Sets the 'index'-th line in 'document' to a copy of 'str'.
 * Note: lines in Document are 0 indexed.
 */
void Document_set_line(Document *document, size_t index, const char *str);

/**
 * Returns the string stored in the 'index'-th line of 'document'.
 */
const char *Document_get_line(Document *document, size_t index);

/**
 * Inserts a copy of 'str' into the 'index'-th line of 'document'.
 *
 * Note: Inserting into the middle of the document shifts all subsequent lines
 * down and inserting past the end fills the gap with empty strings.
 *
 * Inserting str==NULL is invalid. The document can only contain non-null
 * entries
 */
void Document_insert_line(Document *document, size_t index, const char *str);

/**
 * Deletes the 'index'th line line in 'document'.
 *
 * Note: Deleting a line in the middle of the document shifts all subsequent
 * lines up.
 */
void Document_delete_line(Document *document, size_t index);

#endif /* __CS241_DOCUMENT_H__ */
