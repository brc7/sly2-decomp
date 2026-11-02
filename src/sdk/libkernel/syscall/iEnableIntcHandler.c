/* EE kernel syscall stub -92 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iEnableIntcHandler\n"
    ".ent iEnableIntcHandler\n"
    "iEnableIntcHandler:\n"
    "addiu $3, $0, -92\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iEnableIntcHandler\n"
    ".set reorder\n"
);
