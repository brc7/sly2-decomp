/* EE kernel syscall stub 92 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl EnableIntcHandler\n"
    ".ent EnableIntcHandler\n"
    "EnableIntcHandler:\n"
    "addiu $3, $0, 92\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end EnableIntcHandler\n"
    ".set reorder\n"
);
