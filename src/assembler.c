#include "assembler.h"
#include "parser.h"
#include "registers.h"
#include "instr_table.h"
#include "symbols.h"
#include "encode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int assemble_file(const char *in_filename, const char *out_filename) {
    FILE *in = fopen(in_filename, "r");
    if (!in) {
        fprintf(stderr, "Error: Could not open %s\n", in_filename);
        return -1;
    }

    // PASS 1: Build symbol table
    symbols_clear();
    char line[MAX_LINE_LENGTH];
    uint32_t pc = 0;

    int line_num = 0;
    while (fgets(line, sizeof(line), in)) {
        line_num++;
        ParsedLine parsed;
        if (parse_line(line, &parsed) != 0) {
            fprintf(stderr, "Pass 1, Line %d: Parse error\n", line_num);
            fclose(in);
            return -1;
        }

        if (parsed.label[0] != '\0') {
            if (symbols_add(parsed.label, pc) != 0) {
                fprintf(stderr, "Pass 1, Line %d: Error adding label '%s'\n", line_num, parsed.label);
                fclose(in);
                return -1;
            }
        }

        if (parsed.mnemonic[0] != '\0') {
            pc += 4;
        }
    }

    // PASS 2: Encode and output
    rewind(in);
    FILE *out = fopen(out_filename, "w");
    if (!out) {
        fprintf(stderr, "Error: Could not open %s\n", out_filename);
        fclose(in);
        return -1;
    }

    pc = 0;
    line_num = 0;
    while (fgets(line, sizeof(line), in)) {
        line_num++;
        ParsedLine p;
        if (parse_line(line, &p) != 0) {
            fclose(in); fclose(out);
            return -1;
        }

        if (p.mnemonic[0] == '\0') {
            continue;
        }

        const InstrSpec *spec = lookup_instruction(p.mnemonic);
        if (!spec) {
            fprintf(stderr, "Pass 2, Line %d: Unknown instruction '%s'\n", line_num, p.mnemonic);
            fclose(in); fclose(out);
            return -1;
        }

        uint32_t instr_bin = 0;
        int rd = 0, rs1 = 0, rs2 = 0, imm = 0;
        int found;

        if (spec->format == FMT_R) {
            if (p.num_operands != 3) { fprintf(stderr, "Line %d: Expected 3 operands\n", line_num); goto err; }
            rd = parse_register(p.operands[0]);
            rs1 = parse_register(p.operands[1]);
            rs2 = parse_register(p.operands[2]);
            if (rd < 0 || rs1 < 0 || rs2 < 0) { fprintf(stderr, "Line %d: Invalid register\n", line_num); goto err; }
            instr_bin = encode_r(spec->opcode, spec->funct3, spec->funct7, rd, rs1, rs2);
        }
        else if (spec->format == FMT_I) {
            if (spec->opcode == 0x03) { // loads
                if (p.num_operands != 3) { fprintf(stderr, "Line %d: Expected 3 operands\n", line_num); goto err; }
                rd = parse_register(p.operands[0]);
                imm = strtol(p.operands[1], NULL, 0);
                rs1 = parse_register(p.operands[2]);
                if (rd < 0 || rs1 < 0) { fprintf(stderr, "Line %d: Invalid register\n", line_num); goto err; }
                instr_bin = encode_i(spec->opcode, spec->funct3, rd, rs1, imm);
            } else if (spec->opcode == 0x67) { // jalr
                if (p.num_operands != 3) { fprintf(stderr, "Line %d: Expected 3 operands\n", line_num); goto err; }
                rd = parse_register(p.operands[0]);
                rs1 = parse_register(p.operands[1]);
                imm = strtol(p.operands[2], NULL, 0);
                if (rd < 0 || rs1 < 0) { fprintf(stderr, "Line %d: Invalid register\n", line_num); goto err; }
                instr_bin = encode_i(spec->opcode, spec->funct3, rd, rs1, imm);
            } else { // arithmetic I-type
                if (p.num_operands != 3) { fprintf(stderr, "Line %d: Expected 3 operands\n", line_num); goto err; }
                rd = parse_register(p.operands[0]);
                rs1 = parse_register(p.operands[1]);
                imm = strtol(p.operands[2], NULL, 0);
                if (rd < 0 || rs1 < 0) { fprintf(stderr, "Line %d: Invalid register\n", line_num); goto err; }
                
                if (strcmp(p.mnemonic, "slli") == 0 || strcmp(p.mnemonic, "srli") == 0 || strcmp(p.mnemonic, "srai") == 0) {
                    imm = (imm & 0x1F) | (spec->funct7 << 5); // set funct7 in upper bits of imm
                }
                instr_bin = encode_i(spec->opcode, spec->funct3, rd, rs1, imm);
            }
        }
        else if (spec->format == FMT_S) {
            if (p.num_operands != 3) { fprintf(stderr, "Line %d: Expected 3 operands\n", line_num); goto err; }
            rs2 = parse_register(p.operands[0]);
            imm = strtol(p.operands[1], NULL, 0);
            rs1 = parse_register(p.operands[2]);
            if (rs1 < 0 || rs2 < 0) { fprintf(stderr, "Line %d: Invalid register\n", line_num); goto err; }
            instr_bin = encode_s(spec->opcode, spec->funct3, rs1, rs2, imm);
        }
        else if (spec->format == FMT_B) {
            if (p.num_operands != 3) { fprintf(stderr, "Line %d: Expected 3 operands\n", line_num); goto err; }
            rs1 = parse_register(p.operands[0]);
            rs2 = parse_register(p.operands[1]);
            uint32_t target = symbols_find(p.operands[2], &found);
            if (!found) { fprintf(stderr, "Line %d: Undefined label '%s'\n", line_num, p.operands[2]); goto err; }
            if (rs1 < 0 || rs2 < 0) { fprintf(stderr, "Line %d: Invalid register\n", line_num); goto err; }
            imm = (int32_t)(target - pc);
            instr_bin = encode_b(spec->opcode, spec->funct3, rs1, rs2, imm);
        }
        else if (spec->format == FMT_J) {
            if (p.num_operands != 2) { fprintf(stderr, "Line %d: Expected 2 operands\n", line_num); goto err; }
            rd = parse_register(p.operands[0]);
            uint32_t target = symbols_find(p.operands[1], &found);
            if (!found) { fprintf(stderr, "Line %d: Undefined label '%s'\n", line_num, p.operands[1]); goto err; }
            if (rd < 0) { fprintf(stderr, "Line %d: Invalid register\n", line_num); goto err; }
            imm = (int32_t)(target - pc);
            instr_bin = encode_j(spec->opcode, rd, imm);
        }
        else if (spec->format == FMT_U) {
            if (p.num_operands != 2) { fprintf(stderr, "Line %d: Expected 2 operands\n", line_num); goto err; }
            rd = parse_register(p.operands[0]);
            imm = strtol(p.operands[1], NULL, 0);
            if (rd < 0) { fprintf(stderr, "Line %d: Invalid register\n", line_num); goto err; }
            instr_bin = encode_u(spec->opcode, rd, imm);
        }

        fprintf(out, "%08x\n", instr_bin); // hex lowercase output
        pc += 4;
        continue;

    err:
        fclose(in); fclose(out);
        return -1;
    }

    fclose(in);
    fclose(out);
    return 0;
}
