#ifndef ASSEMBLER_H
#define ASSEMBLER_H

// Assembles an input assembly file into an output machine code file.
// Returns 0 on success, non-zero on error.
int assemble_file(const char *in_filename, const char *out_filename);

#endif // ASSEMBLER_H
