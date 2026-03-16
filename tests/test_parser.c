#include <stdio.h>
#include <string.h>
#include "parser.h"

int test_line(const char *line, const char *exp_label, const char *exp_mnemonic, int exp_ops, const char **exp_operands) {
    ParsedLine p;
    if (parse_line(line, &p) != 0) {
        printf("FAIL: parse_line returned error for: %s\n", line);
        return 1;
    }
    
    if (strcmp(p.label, exp_label) != 0) {
        printf("FAIL: Label mismatch for '%s'. Expected '%s', got '%s'\n", line, exp_label, p.label);
        return 1;
    }
    if (strcmp(p.mnemonic, exp_mnemonic) != 0) {
        printf("FAIL: Mnemonic mismatch for '%s'. Expected '%s', got '%s'\n", line, exp_mnemonic, p.mnemonic);
        return 1;
    }
    if (p.num_operands != exp_ops) {
        printf("FAIL: Num operands mismatch for '%s'. Expected %d, got %d\n", line, exp_ops, p.num_operands);
        return 1;
    }
    for (int i=0; i<exp_ops; i++) {
        if (strcmp(p.operands[i], exp_operands[i]) != 0) {
            printf("FAIL: Operand %d mismatch for '%s'. Expected '%s', got '%s'\n", i, line, exp_operands[i], p.operands[i]);
            return 1;
        }
    }
    return 0;
}

int main() {
    int failed = 0;
    
    // R-type with label
    const char *ops1[] = {"x1", "x2", "x3"};
    failed += test_line("loop: add x1, x2, x3", "loop", "add", 3, ops1);
    
    // I-type with comment
    const char *ops2[] = {"x1", "x0", "5"};
    failed += test_line("  addi x1 x0 5 # comment", "", "addi", 3, ops2);
    
    // Label only
    failed += test_line("start:", "start", "", 0, NULL);
    
    // Branch
    const char *ops4[] = {"x1", "x2", "loop"};
    failed += test_line("beq x1, x2, loop", "", "beq", 3, ops4);
    
    // Memory with parentheses
    const char *ops5[] = {"x1", "0", "x2"};
    failed += test_line("lw x1, 0(x2)", "", "lw", 3, ops5);

    // No operands
    failed += test_line("ecall", "", "ecall", 0, NULL);

    if (failed == 0) {
        printf("PASS: All parser tests passed.\n");
        return 0;
    } else {
        printf("FAIL: %d parser tests failed.\n", failed);
        return 1;
    }
}
