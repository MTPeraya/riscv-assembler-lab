#include <stdio.h>
#include "assembler.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input.s> <output.hex>\n", argv[0]);
        return 1;
    }

    int ret = assemble_file(argv[1], argv[2]);
    if (ret == 0) {
        printf("Assembly successful. Output written to %s\n", argv[2]);
    } else {
        fprintf(stderr, "Assembly failed.\n");
    }

    return ret;
}
