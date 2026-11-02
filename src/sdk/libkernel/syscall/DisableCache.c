/* EE kernel syscall stub 98 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl DisableCache\n"
    ".ent DisableCache\n"
    "DisableCache:\n"
    "addiu $3, $0, 98\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end DisableCache\n"
    ".set reorder\n"
);
