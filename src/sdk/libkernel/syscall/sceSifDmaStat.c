/* EE kernel syscall stub 118 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl sceSifDmaStat\n"
    ".ent sceSifDmaStat\n"
    "sceSifDmaStat:\n"
    "addiu $3, $0, 118\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end sceSifDmaStat\n"
    ".set reorder\n"
);
