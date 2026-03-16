# RISC-V Assembler Lab

This repository contains a fully functional, two-pass generic assembler for a subset of the **RV32I (RISC-V 32-bit Integer)** instruction set architecture. It was built in `C` utilizing modular software design principles with unit tests verifying internal components.

## Supported Instructions
The assembler supports instructions mapped out by the CS61C reference card configurations encompassing the following layouts:

- **R-type (Register-Register Arithmetic)**: `add`, `sub`, `and`, `or`, `xor`, `sll`, `srl`, `sra`, `slt`, `sltu`
- **I-type (Immediate Arithmetic)**: `addi`, `andi`, `ori`, `xori`, `slli`, `srli`, `srai`, `slti`, `sltiu`
- **I-type (Memory Loads)**: `lb`, `lbu`, `lh`, `lhu`, `lw`
- **S-type (Memory Stores)**: `sb`, `sh`, `sw`
- **B-type (Conditional Branches)**: `beq`, `bne`, `blt`, `bge`, `bltu`, `bgeu`
- **J-type (Unconditional Jumps)**: `jal`, `jalr`

## Modular Repository Structure

The assembler relies on breaking apart text processing and bitwise generation. Every sub-component enforces loose coupling, unit tested independently. 

- `src/registers.c`: Converts string register identifiers (e.g., `"x5"`) into their raw indices `5`.
- `src/parser.c`: Handles raw instruction string iteration and identifies labels, targets, operations, stripping comments away.
- `src/instr_table.c`: Functions effectively as a data layout structure querying formats (`R`, `I`, `S`, `B`, `J`), raw `opcodes`, and `funct3`/`funct7` fields by parsing instruction strings. 
- `src/symbols.c`: Maintains label strings referencing `start`, `done`, and maps logical index line values ensuring valid relative offsets on branching.
- `src/encode.c`: Houses bit-packing methods shifting opcodes, formats, and immediates generating the final 32-bit `0xXXXXXXXX` output binaries matching instruction formats. 
- `src/assembler.c`: Orchestrates a **two-pass procedure**.
  - Pass 1 scopes logic, counting program counters (`PC`) offsets, appending un-initialized target labels to the index array.
  - Pass 2 parses logic against the target indices verifying distance gaps via target `PC` - current `PC` and pushing inputs to `encode.c`.
- `src/main.c`: Final CLI executable logic managing `<input.s> <output.hex>`.

## Execution

### Compilation
Build the test suites along with the main assembler:
```bash
make clean
make assembler
```

### Usage
Generate object `.hex` files using string matching paths targeting local assembly routines:
```bash
./assembler <input_file.s> <output_file.hex>
```

#### Example Usage
```bash
./assembler examples/test_case_3.s examples/test_case_3.hex
```

## Running the Unit Tests
Test coverage ensures reliable component parsing across edge cases. Compile natively executing via:
```bash
make test_registers && ./test_registers
make test_parser && ./test_parser
make test_instr_table && ./test_instr_table
make test_symbols && ./test_symbols
make test_encode && ./test_encode
make test_assembler && ./test_assembler
```

## Example Tests / Verification
Under `examples/`, three robust RISC-V workloads are located implementing algorithm iterations and diverse command testing.
- `test_case_1.s` *(~22 Lines)* - Vector Logic Addition iterating across distinct memory matrices appending target sizes.
- `test_case_2.s` *(~55 Lines)* - Fibonacci calculation loop iterating arrays storing values natively validating logical shifts/edge branchings dynamically.
- `test_case_3.s` *(>120 Lines)* - Fully robust iterative Bubble Sort execution hitting boundary constraints and dummy bit filling ensuring comprehensive instruction evaluation layout outputs. 

To run the output inside of the standard RISC-V environment emulator (e.g. Venus), open the target `test_case_X.s` configuration into the terminal emulator interface mimicking native architecture executions matching logical output states mapped by the hexadecimal values stored natively in the generic `.hex` build outputs.
