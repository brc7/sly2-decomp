/* EE kernel syscall stub 16 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl AddIntcHandler2\n"
    ".ent AddIntcHandler2\n"
    "AddIntcHandler2:\n"
    "addiu $3, $0, 16\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end AddIntcHandler2\n"
    ".set reorder\n"
);
