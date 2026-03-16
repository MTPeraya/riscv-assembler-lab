#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>

// Packs fields into an R-type instruction
uint32_t encode_r(uint32_t opcode, uint32_t funct3, uint32_t funct7, uint32_t rd, uint32_t rs1, uint32_t rs2);

// Packs fields into an I-type instruction
uint32_t encode_i(uint32_t opcode, uint32_t funct3, uint32_t rd, uint32_t rs1, uint32_t imm);

// Packs fields into an S-type instruction
uint32_t encode_s(uint32_t opcode, uint32_t funct3, uint32_t rs1, uint32_t rs2, uint32_t imm);

// Packs fields into a B-type instruction
uint32_t encode_b(uint32_t opcode, uint32_t funct3, uint32_t rs1, uint32_t rs2, uint32_t imm);

// Packs fields into a U-type instruction
uint32_t encode_u(uint32_t opcode, uint32_t rd, uint32_t imm);

// Packs fields into a J-type instruction
uint32_t encode_j(uint32_t opcode, uint32_t rd, uint32_t imm);

#endif // ENCODE_H
