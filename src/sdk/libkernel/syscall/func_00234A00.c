/* EE kernel syscall stub 62 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234A00\n"
    ".ent func_00234A00\n"
    "func_00234A00:\n"
    "addiu $3, $0, 62\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234A00\n"
    ".set reorder\n"
);
