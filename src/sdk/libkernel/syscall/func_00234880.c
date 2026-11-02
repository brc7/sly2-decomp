/* EE kernel syscall stub -38 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234880\n"
    ".ent func_00234880\n"
    "func_00234880:\n"
    "addiu $3, $0, -38\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234880\n"
    ".set reorder\n"
);
