#ifndef INSTR_TABLE_H
#define INSTR_TABLE_H

#include <stdint.h>

typedef enum {
    FMT_R,
    FMT_I,
    FMT_S,
    FMT_B,
    FMT_U,
    FMT_J,
    FMT_NONE
} InstrFormat;

typedef struct {
    const char *mnemonic;
    InstrFormat format;
    uint32_t opcode;
    uint32_t funct3;
    uint32_t funct7;
} InstrSpec;

// Looks up an instruction by its mnemonic.
// Returns a pointer to its specification, or NULL if not found.
const InstrSpec *lookup_instruction(const char *mnemonic);

#endif // INSTR_TABLE_H
