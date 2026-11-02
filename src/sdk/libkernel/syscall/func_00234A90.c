/* EE kernel syscall stub 71 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234A90\n"
    ".ent func_00234A90\n"
    "func_00234A90:\n"
    "addiu $3, $0, 71\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234A90\n"
    ".set reorder\n"
);
