#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int parse_line(const char *line, ParsedLine *out) {
    out->label[0] = '\0';
    out->mnemonic[0] = '\0';
    out->num_operands = 0;
    for (int i=0; i<MAX_OPERANDS; i++) out->operands[i][0] = '\0';

    char buffer[MAX_LINE_LENGTH];
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';

    // 1. Strip comments (everything after #)
    char *comment = strchr(buffer, '#');
    if (comment) {
        *comment = '\0';
    }

    char *start = buffer;
    // 2. Extract label (if any)
    char *colon = strchr(start, ':');
    if (colon) {
        *colon = '\0';
        char *l = start;
        while (*l && isspace((unsigned char)*l)) l++;
        
        char *end = colon - 1;
        while (end >= l && isspace((unsigned char)*end)) {
            *end = '\0';
            end--;
        }
        strncpy(out->label, l, MAX_LINE_LENGTH - 1);
        start = colon + 1;
    }

    // 3. Extract mnemonic
    while (*start && isspace((unsigned char)*start)) start++;
    if (*start == '\0') {
        return 0; // Empty line or just label
    }
    
    char *m_end = start;
    while (*m_end && !isspace((unsigned char)*m_end)) m_end++;
    if (*m_end != '\0') {
        *m_end = '\0';
        strncpy(out->mnemonic, start, MAX_LINE_LENGTH - 1);
        start = m_end + 1;
    } else {
        strncpy(out->mnemonic, start, MAX_LINE_LENGTH - 1);
        return 0; // No operands
    }

    // 4. Extract operands separated by spaces, commas, or parentheses (for lw/sw offsets)
    while (*start) {
        while (*start && (isspace((unsigned char)*start) || *start == ',' || *start == '(' || *start == ')')) start++;
        if (*start == '\0') break;

        char *o_end = start;
        while (*o_end && !isspace((unsigned char)*o_end) && *o_end != ',' && *o_end != '(' && *o_end != ')') o_end++;

        if (out->num_operands >= MAX_OPERANDS) {
            return -1; // Too many operands
        }
        
        char saved = *o_end;
        *o_end = '\0';
        strncpy(out->operands[out->num_operands], start, MAX_LINE_LENGTH - 1);
        out->num_operands++;

        if (saved == '\0') break;
        start = o_end + 1;
    }

    return 0;
}
