#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

int main() {
    FILE *f = fopen("dummy.s", "w");
    fprintf(f, "start:\n");
    fprintf(f, "  addi x1, x0, 5\n");
    fprintf(f, "  add x2, x1, x1\n");
    fprintf(f, "  beq x1, x2, start\n");
    fclose(f);

    if (assemble_file("dummy.s", "dummy.hex") != 0) {
        printf("FAIL: assemble_file returned error\n");
        return 1;
    }

    f = fopen("dummy.hex", "r");
    if (!f) {
        printf("FAIL: could not open output hex\n");
        return 1;
    }

    unsigned int v1, v2, v3;
    if (fscanf(f, "%x", &v1) != 1 || v1 != 0x00500093) {
        printf("FAIL: instruction 1 mismatch: 0x%08x\n", v1); return 1;
    }
    if (fscanf(f, "%x", &v2) != 1 || v2 != 0x00108133) {
        printf("FAIL: instruction 2 mismatch: 0x%08x\n", v2); return 1;
    }
    if (fscanf(f, "%x", &v3) != 1 || v3 != 0xfe208ce3) {
        printf("FAIL: instruction 3 mismatch: 0x%08x\n", v3); return 1;
    }

    fclose(f);
    printf("PASS: Assembler pipeline works.\n");
    return 0;
}
