/* EE kernel syscall stub 12 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_002346C0\n"
    ".ent func_002346C0\n"
    "func_002346C0:\n"
    "addiu $3, $0, 12\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_002346C0\n"
    ".set reorder\n"
);
