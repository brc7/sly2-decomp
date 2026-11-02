/* EE kernel syscall stub 9 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234690\n"
    ".ent func_00234690\n"
    "func_00234690:\n"
    "addiu $3, $0, 9\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234690\n"
    ".set reorder\n"
);
