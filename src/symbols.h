#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdint.h>

#define MAX_LABELS 256
#define MAX_LABEL_LEN 256

// Adds a label and its address to the symbol table.
// Returns 0 on success, -1 on failure (e.g., table full, duplicate label).
int symbols_add(const char *name, uint32_t address);

// Finds the address of a given label.
// Sets *found to 1 if found, 0 otherwise.
// Returns the address if found, 0 otherwise.
uint32_t symbols_find(const char *name, int *found);

// Clears the symbol table for testing or multiple passes.
void symbols_clear(void);

#endif // SYMBOLS_H
