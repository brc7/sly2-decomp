/* EE kernel syscall stub 114 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetPgifHandler\n"
    ".ent SetPgifHandler\n"
    "SetPgifHandler:\n"
    "addiu $3, $0, 114\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetPgifHandler\n"
    ".set reorder\n"
);
