/* EE kernel syscall stub -93 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234C10\n"
    ".ent func_00234C10\n"
    "func_00234C10:\n"
    "addiu $3, $0, -93\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234C10\n"
    ".set reorder\n"
);
