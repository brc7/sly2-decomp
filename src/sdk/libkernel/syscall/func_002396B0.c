/* EE kernel syscall stub 116 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_002396B0\n"
    ".ent func_002396B0\n"
    "func_002396B0:\n"
    "addiu $3, $0, 116\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_002396B0\n"
    ".set reorder\n"
);
