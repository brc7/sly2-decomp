/* EE kernel syscall stub 100 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl FlushCache\n"
    ".ent FlushCache\n"
    "FlushCache:\n"
    "addiu $3, $0, 100\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end FlushCache\n"
    ".set reorder\n"
);
