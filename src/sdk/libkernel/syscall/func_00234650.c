/* EE kernel syscall stub 5 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234650\n"
    ".ent func_00234650\n"
    "func_00234650:\n"
    "addiu $3, $0, 5\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234650\n"
    ".set reorder\n"
);
