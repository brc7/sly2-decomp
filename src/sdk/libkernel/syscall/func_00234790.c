/* EE kernel syscall stub 23 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234790\n"
    ".ent func_00234790\n"
    "func_00234790:\n"
    "addiu $3, $0, 23\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234790\n"
    ".set reorder\n"
);
