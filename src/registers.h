#ifndef REGISTERS_H
#define REGISTERS_H

// Parses a RISC-V register name and returns its number (0-31).
// Returns -1 if the input is not a valid register name.
int parse_register(const char *name);

#endif // REGISTERS_H
