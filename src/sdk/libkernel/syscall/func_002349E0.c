/* EE kernel syscall stub 60 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_002349E0\n"
    ".ent func_002349E0\n"
    "func_002349E0:\n"
    "addiu $3, $0, 60\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_002349E0\n"
    ".set reorder\n"
);
