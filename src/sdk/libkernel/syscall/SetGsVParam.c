/* EE kernel syscall stub 79 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetGsVParam\n"
    ".ent SetGsVParam\n"
    "SetGsVParam:\n"
    "addiu $3, $0, 79\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetGsVParam\n"
    ".set reorder\n"
);
