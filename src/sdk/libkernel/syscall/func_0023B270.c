/* EE kernel syscall stub 91 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_0023B270\n"
    ".ent func_0023B270\n"
    "func_0023B270:\n"
    "addiu $3, $0, 91\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_0023B270\n"
    ".set reorder\n"
);
