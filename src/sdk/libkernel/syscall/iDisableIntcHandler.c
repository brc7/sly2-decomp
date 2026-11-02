/* EE kernel syscall stub -93 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iDisableIntcHandler\n"
    ".ent iDisableIntcHandler\n"
    "iDisableIntcHandler:\n"
    "addiu $3, $0, -93\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iDisableIntcHandler\n"
    ".set reorder\n"
);
