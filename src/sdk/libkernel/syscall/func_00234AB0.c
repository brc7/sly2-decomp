/* EE kernel syscall stub 73 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234AB0\n"
    ".ent func_00234AB0\n"
    "func_00234AB0:\n"
    "addiu $3, $0, 73\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234AB0\n"
    ".set reorder\n"
);
