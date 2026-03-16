#include <stdio.h>
#include "instr_table.h"

int main() {
    int failed = 0;

    const InstrSpec *spec;

    spec = lookup_instruction("add");
    if (!spec || spec->format != FMT_R || spec->opcode != 0x33 || spec->funct3 != 0x0 || spec->funct7 != 0x00) {
        printf("FAIL: 'add' lookup incorrect or not found.\n");
        failed++;
    }

    spec = lookup_instruction("addi");
    if (!spec || spec->format != FMT_I || spec->opcode != 0x13 || spec->funct3 != 0x0) {
        printf("FAIL: 'addi' lookup incorrect or not found.\n");
        failed++;
    }

    spec = lookup_instruction("sw");
    if (!spec || spec->format != FMT_S || spec->opcode != 0x23 || spec->funct3 != 0x2) {
        printf("FAIL: 'sw' lookup incorrect or not found.\n");
        failed++;
    }

    spec = lookup_instruction("bgeu");
    if (!spec || spec->format != FMT_B || spec->opcode != 0x63 || spec->funct3 != 0x7) {
        printf("FAIL: 'bgeu' lookup incorrect or not found.\n");
        failed++;
    }

    spec = lookup_instruction("jal");
    if (!spec || spec->format != FMT_J || spec->opcode != 0x6f) {
        printf("FAIL: 'jal' lookup incorrect or not found.\n");
        failed++;
    }

    spec = lookup_instruction("invalid_instr");
    if (spec != NULL) {
        printf("FAIL: 'invalid_instr' should return NULL.\n");
        failed++;
    }

    if (failed == 0) {
        printf("PASS: All instruction table tests passed.\n");
        return 0;
    } else {
        printf("FAIL: %d instruction table tests failed.\n", failed);
        return 1;
    }
}
