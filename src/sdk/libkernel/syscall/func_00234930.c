/* EE kernel syscall stub -49 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234930\n"
    ".ent func_00234930\n"
    "func_00234930:\n"
    "addiu $3, $0, -49\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234930\n"
    ".set reorder\n"
);
