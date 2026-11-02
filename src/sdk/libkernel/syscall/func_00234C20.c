/* EE kernel syscall stub 94 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234C20\n"
    ".ent func_00234C20\n"
    "func_00234C20:\n"
    "addiu $3, $0, 94\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234C20\n"
    ".set reorder\n"
);
