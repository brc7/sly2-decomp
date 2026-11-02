/* EE kernel syscall stub 39 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234890\n"
    ".ent func_00234890\n"
    "func_00234890:\n"
    "addiu $3, $0, 39\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234890\n"
    ".set reorder\n"
);
