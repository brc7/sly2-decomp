/* EE kernel syscall stub -104 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iFlushCache\n"
    ".ent iFlushCache\n"
    "iFlushCache:\n"
    "addiu $3, $0, -104\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iFlushCache\n"
    ".set reorder\n"
);
