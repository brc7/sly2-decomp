/* EE kernel syscall stub 65 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234A30\n"
    ".ent func_00234A30\n"
    "func_00234A30:\n"
    "addiu $3, $0, 65\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234A30\n"
    ".set reorder\n"
);
