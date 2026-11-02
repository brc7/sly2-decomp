/* EE kernel syscall stub -54 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234980\n"
    ".ent func_00234980\n"
    "func_00234980:\n"
    "addiu $3, $0, -54\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234980\n"
    ".set reorder\n"
);
