/* EE kernel syscall stub -83 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234B50\n"
    ".ent func_00234B50\n"
    "func_00234B50:\n"
    "addiu $3, $0, -83\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234B50\n"
    ".set reorder\n"
);
