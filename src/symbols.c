#include "symbols.h"
#include <string.h>

typedef struct {
    char name[MAX_LABEL_LEN];
    uint32_t address;
} Symbol;

static Symbol symbol_table[MAX_LABELS];
static int num_symbols = 0;

void symbols_clear(void) {
    num_symbols = 0;
}

int symbols_add(const char *name, uint32_t address) {
    if (!name || num_symbols >= MAX_LABELS) {
        return -1;
    }

    int found;
    symbols_find(name, &found);
    if (found) {
        return -1; // Duplicate label
    }

    strncpy(symbol_table[num_symbols].name, name, MAX_LABEL_LEN - 1);
    symbol_table[num_symbols].name[MAX_LABEL_LEN - 1] = '\0';
    symbol_table[num_symbols].address = address;
    num_symbols++;

    return 0;
}

uint32_t symbols_find(const char *name, int *found) {
    *found = 0;
    if (!name) return 0;

    for (int i = 0; i < num_symbols; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            *found = 1;
            return symbol_table[i].address;
        }
    }

    return 0;
}
