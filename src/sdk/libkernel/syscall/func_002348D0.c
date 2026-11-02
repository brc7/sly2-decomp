/* EE kernel syscall stub 43 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_002348D0\n"
    ".ent func_002348D0\n"
    "func_002348D0:\n"
    "addiu $3, $0, 43\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_002348D0\n"
    ".set reorder\n"
);
