/* EE kernel syscall stub 131 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00239598\n"
    ".ent func_00239598\n"
    "func_00239598:\n"
    "addiu $3, $0, 131\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00239598\n"
    ".set reorder\n"
);
