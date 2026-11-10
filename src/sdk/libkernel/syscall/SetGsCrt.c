/* EE kernel syscall stub 2 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetGsCrt\n"
    ".ent SetGsCrt\n"
    "SetGsCrt:\n"
    "addiu $3, $0, 2\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetGsCrt\n"
    ".set reorder\n"
);
