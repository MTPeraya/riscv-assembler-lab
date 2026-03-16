# Test Case 2: >50 lines
# Fibonacci sequence calculation and memory store
# Also tests branching, arithmetic, and shifts

start:
    # Initialize parameters
    addi x1, x0, 10      # Calculate 10 Fibonacci numbers
    addi x2, x0, 0x200   # Memory base to store them
    addi x3, x0, 0       # f[n-2]
    addi x4, x0, 1       # f[n-1]
    
    # Store f[0] and f[1]
    sw x3, 0(x2)
    sw x4, 4(x2)

    # Setup loop
    addi x5, x0, 2       # i = 2
    addi x6, x0, 8       # Memory offset initialized for f[2]

fib_loop:
    # Branch out if we hit the limit
    bge x5, x1, end_fib

    # Calculating next term
    add x7, x3, x4       # f[n] = f[n-2] + f[n-1]
    
    # Memory operation
    add x8, x2, x6       # memory address
    sw x7, 0(x8)         # store it

    # Update previous terms
    add x3, x0, x4       # f[n-2] = f[n-1]
    add x4, x0, x7       # f[n-1] = f[n]

    # Arithmetic updates
    addi x5, x5, 1       # i++
    addi x6, x6, 4       # memory offset advances by 4

    # Exercise logic instructions
    and x9, x5, x1
    or x10, x5, x1
    xor x11, x5, x1
    
    # Immediate logic
    andi x12, x5, 1
    ori x13, x5, 2
    xori x14, x5, 4

    # Bitwise shifts
    slli x15, x5, 1
    srli x16, x5, 1
    srai x17, x5, 1
    
    # Comparison
    slt x18, x5, x1
    sltu x19, x5, x1
    slti x20, x5, 20
    sltiu x21, x5, 20

    # Test load instructions purely to have them
    lb x22, 0(x2)
    lbu x23, 0(x2)
    lh x24, 0(x2)
    lhu x25, 0(x2)
    
    # Test branch variants (unlikely to trigger)
    blt x0, x0, fake_target
    bne x0, x0, fake_target
    bltu x0, x0, fake_target
    bgeu x1, x0, next_line  # This one should be taken immediately
    
fake_target:
    addi x0, x0, 0

next_line:
    jal x0, fib_loop

end_fib:
    addi x1, x0, 0
    jalr x0, x1, 0
