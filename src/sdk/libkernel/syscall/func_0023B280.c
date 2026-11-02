/* EE kernel syscall stub 116 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_0023B280\n"
    ".ent func_0023B280\n"
    "func_0023B280:\n"
    "addiu $3, $0, 116\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_0023B280\n"
    ".set reorder\n"
);
