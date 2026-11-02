/* EE kernel syscall stub 78 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetGsHParam\n"
    ".ent SetGsHParam\n"
    "SetGsHParam:\n"
    "addiu $3, $0, 78\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetGsHParam\n"
    ".set reorder\n"
);
