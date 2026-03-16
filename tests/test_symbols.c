#include <stdio.h>
#include "symbols.h"

int main() {
    int failed = 0;

    symbols_clear();

    if (symbols_add("loop", 12) != 0) {
        printf("FAIL: Failed to add 'loop'\n");
        failed++;
    }

    if (symbols_add("end", 24) != 0) {
        printf("FAIL: Failed to add 'end'\n");
        failed++;
    }

    if (symbols_add("loop", 36) == 0) {
        printf("FAIL: Should not be able to add duplicate 'loop'\n");
        failed++;
    }

    int found;
    uint32_t addr;

    addr = symbols_find("loop", &found);
    if (!found || addr != 12) {
        printf("FAIL: Failed to find 'loop' or address mismatch\n");
        failed++;
    }

    addr = symbols_find("end", &found);
    if (!found || addr != 24) {
        printf("FAIL: Failed to find 'end' or address mismatch\n");
        failed++;
    }

    addr = symbols_find("missing", &found);
    if (found) {
        printf("FAIL: Found non-existent symbol 'missing'\n");
        failed++;
    }

    if (failed == 0) {
        printf("PASS: All symbol tests passed.\n");
        return 0;
    } else {
        printf("FAIL: %d symbol tests failed.\n", failed);
        return 1;
    }
}
