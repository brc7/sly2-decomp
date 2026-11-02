/* EE kernel syscall stub 96 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234C60\n"
    ".ent func_00234C60\n"
    "func_00234C60:\n"
    "addiu $3, $0, 96\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234C60\n"
    ".set reorder\n"
);
