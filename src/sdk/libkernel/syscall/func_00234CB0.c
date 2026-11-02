/* EE kernel syscall stub 102 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234CB0\n"
    ".ent func_00234CB0\n"
    "func_00234CB0:\n"
    "addiu $3, $0, 102\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234CB0\n"
    ".set reorder\n"
);
