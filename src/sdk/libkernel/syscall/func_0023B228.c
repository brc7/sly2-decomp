/* EE kernel syscall stub 90 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_0023B228\n"
    ".ent func_0023B228\n"
    "func_0023B228:\n"
    "addiu $3, $0, 90\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_0023B228\n"
    ".set reorder\n"
);
