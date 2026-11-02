/* EE kernel syscall stub 89 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234BB0\n"
    ".ent func_00234BB0\n"
    "func_00234BB0:\n"
    "addiu $3, $0, 89\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234BB0\n"
    ".set reorder\n"
);
