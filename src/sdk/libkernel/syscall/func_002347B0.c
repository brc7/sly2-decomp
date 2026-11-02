/* EE kernel syscall stub 253 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_002347B0\n"
    ".ent func_002347B0\n"
    "func_002347B0:\n"
    "addiu $3, $0, 253\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_002347B0\n"
    ".set reorder\n"
);
