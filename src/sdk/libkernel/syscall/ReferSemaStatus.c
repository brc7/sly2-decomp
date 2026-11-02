/* EE kernel syscall stub 71 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl ReferSemaStatus\n"
    ".ent ReferSemaStatus\n"
    "ReferSemaStatus:\n"
    "addiu $3, $0, 71\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end ReferSemaStatus\n"
    ".set reorder\n"
);
