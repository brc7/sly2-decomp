/* EE kernel syscall stub 252 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_002347A0\n"
    ".ent func_002347A0\n"
    "func_002347A0:\n"
    "addiu $3, $0, 252\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_002347A0\n"
    ".set reorder\n"
);
