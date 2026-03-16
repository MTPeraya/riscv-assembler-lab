#include <stdio.h>
#include "encode.h"

int main() {
    int failed = 0;
    
    // add x1, x2, x3 -> 0000000_00011_00010_000_00001_0110011 -> 0x003100b3
    if (encode_r(0x33, 0x0, 0x0, 1, 2, 3) != 0x003100b3) {
        printf("FAIL: encode_r failed. Got 0x%08x\n", encode_r(0x33, 0x0, 0x0, 1, 2, 3));
        failed++;
    }

    // addi x1, x2, 5 -> 000000000101_00010_000_00001_0010011 -> 0x00510093
    if (encode_i(0x13, 0x0, 1, 2, 5) != 0x00510093) {
        printf("FAIL: encode_i failed. Got 0x%08x\n", encode_i(0x13, 0x0, 1, 2, 5));
        failed++;
    }

    // sw x2, 0(x1) -> 0000000_00010_00001_010_00000_0100011 -> 0x0020a023
    if (encode_s(0x23, 0x2, 1, 2, 0) != 0x0020a023) {
        printf("FAIL: encode_s failed. Got 0x%08x\n", encode_s(0x23, 0x2, 1, 2, 0));
        failed++;
    }

    // beq x1, x2, 4 -> 0000000_00010_00001_000_0010_0_1100011 -> 0x00208263
    if (encode_b(0x63, 0x0, 1, 2, 4) != 0x00208263) {
        printf("FAIL: encode_b failed. Got 0x%08x\n", encode_b(0x63, 0x0, 1, 2, 4));
        failed++;
    }

    // jal x1, 4 -> 0x004000ef
    if (encode_j(0x6f, 1, 4) != 0x004000ef) {
        printf("FAIL: encode_j failed. Got 0x%08x\n", encode_j(0x6f, 1, 4));
        failed++;
    }

    if (failed == 0) {
        printf("PASS: All encode tests passed.\n");
        return 0;
    } else {
        printf("FAIL: %d encode tests failed.\n", failed);
        return 1;
    }
}
