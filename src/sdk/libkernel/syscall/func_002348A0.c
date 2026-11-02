/* EE kernel syscall stub 40 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_002348A0\n"
    ".ent func_002348A0\n"
    "func_002348A0:\n"
    "addiu $3, $0, 40\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_002348A0\n"
    ".set reorder\n"
);
