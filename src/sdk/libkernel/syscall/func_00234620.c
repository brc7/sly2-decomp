/* EE kernel syscall stub 2 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234620\n"
    ".ent func_00234620\n"
    "func_00234620:\n"
    "addiu $3, $0, 2\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234620\n"
    ".set reorder\n"
);
