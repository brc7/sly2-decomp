/* EE kernel syscall stub 93 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl DisableIntcHandler\n"
    ".ent DisableIntcHandler\n"
    "DisableIntcHandler:\n"
    "addiu $3, $0, 93\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end DisableIntcHandler\n"
    ".set reorder\n"
);
