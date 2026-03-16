#include <stdio.h>
#include "registers.h"

int main() {
    int failed = 0;
    
    struct {
        const char *name;
        int expected;
    } tests[] = {
        {"x0", 0},
        {"x5", 5},
        {"x10", 10},
        {"x31", 31},
        {"x32", -1},
        {"x-1", -1},
        {"abc", -1},
        {"x", -1},
        {"", -1},
        {NULL, -1}
    };
    
    int num_tests = sizeof(tests) / sizeof(tests[0]);
    
    for (int i = 0; i < num_tests; i++) {
        int result = parse_register(tests[i].name);
        if (result != tests[i].expected) {
            printf("FAIL: parse_register(\"%s\") returned %d, expected %d\n", 
                   tests[i].name ? tests[i].name : "NULL", result, tests[i].expected);
            failed++;
        }
    }
    
    if (failed == 0) {
        printf("PASS: All register tests passed.\n");
        return 0;
    } else {
        printf("FAIL: %d register tests failed.\n", failed);
        return 1;
    }
}
