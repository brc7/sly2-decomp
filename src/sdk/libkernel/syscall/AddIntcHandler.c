/* EE kernel syscall stub 16 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl AddIntcHandler\n"
    ".ent AddIntcHandler\n"
    "AddIntcHandler:\n"
    "addiu $3, $0, 16\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end AddIntcHandler\n"
    ".set reorder\n"
);
