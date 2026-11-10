/* EE kernel syscall stub 74 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetOsdConfigParam\n"
    ".ent SetOsdConfigParam\n"
    "SetOsdConfigParam:\n"
    "addiu $3, $0, 74\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetOsdConfigParam\n"
    ".set reorder\n"
);
