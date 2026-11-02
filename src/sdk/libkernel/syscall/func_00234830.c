/* EE kernel syscall stub 33 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234830\n"
    ".ent func_00234830\n"
    "func_00234830:\n"
    "addiu $3, $0, 33\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234830\n"
    ".set reorder\n"
);
