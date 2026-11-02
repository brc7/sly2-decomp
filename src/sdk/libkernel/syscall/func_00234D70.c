/* EE kernel syscall stub -113 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234D70\n"
    ".ent func_00234D70\n"
    "func_00234D70:\n"
    "addiu $3, $0, -113\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234D70\n"
    ".set reorder\n"
);
