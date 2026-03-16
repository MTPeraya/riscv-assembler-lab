#ifndef PARSER_H
#define PARSER_H

#define MAX_LINE_LENGTH 256
#define MAX_OPERANDS 4

typedef struct {
    char label[MAX_LINE_LENGTH];     // Empty string if no label
    char mnemonic[MAX_LINE_LENGTH];  // Empty string if no instruction
    char operands[MAX_OPERANDS][MAX_LINE_LENGTH]; // Operands
    int num_operands;
} ParsedLine;

// Parses a line of assembly text into a ParsedLine structure.
// Returns 0 on success, -1 on a parsing error.
int parse_line(const char *line, ParsedLine *out);

#endif // PARSER_H
