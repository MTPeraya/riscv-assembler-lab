# Test Case 1: >20 lines
# Vector Addition (A + B = C) for 4 elements

start:
    # Initialize array pointers and size
    addi x10, x0, 0x100 # A array base address
    addi x11, x0, 0x110 # B array base address
    addi x12, x0, 0x120 # C array base address
    addi x13, x0, 4     # Array size N = 4
    addi x14, x0, 0     # Index i = 0

loop:
    # Check if loop is done: i >= N
    bge x14, x13, done

    # Calculate offset (i * 4)
    slli x15, x14, 2
    
    # Load A[i]
    add x16, x10, x15
    lw x17, 0(x16)
    
    # Load B[i]
    add x16, x11, x15
    lw x18, 0(x16)

    # C[i] = A[i] + B[i]
    add x19, x17, x18
    
    # Store C[i]
    add x16, x12, x15
    sw x19, 0(x16)

    # Increment i
    addi x14, x14, 1
    
    # Repeat
    jal x0, loop

done:
    # End program logically
    addi x10, x0, 10
    add x0, x0, x0
