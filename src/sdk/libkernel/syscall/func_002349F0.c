/* EE kernel syscall stub 61 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_002349F0\n"
    ".ent func_002349F0\n"
    "func_002349F0:\n"
    "addiu $3, $0, 61\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_002349F0\n"
    ".set reorder\n"
);
