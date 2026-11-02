/* EE kernel syscall stub 15 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_002346F0\n"
    ".ent func_002346F0\n"
    "func_002346F0:\n"
    "addiu $3, $0, 15\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_002346F0\n"
    ".set reorder\n"
);
