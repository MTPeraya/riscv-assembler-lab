#include "registers.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int parse_register(const char *name) {
    if (!name || name[0] != 'x') {
        return -1; // Basic support for x0-x31
    }
    
    // Make sure the rest of the string is strictly digits
    int i = 1;
    if (name[i] == '\0') {
        return -1; // Just "x"
    }
    
    while (name[i] != '\0') {
        if (!isdigit((unsigned char)name[i])) {
            return -1;
        }
        i++;
    }
    
    int reg_num = atoi(&name[1]);
    if (reg_num >= 0 && reg_num <= 31) {
        return reg_num;
    }
    
    return -1;
}
