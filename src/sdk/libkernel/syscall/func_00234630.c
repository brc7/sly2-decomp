/* EE kernel syscall stub 3 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234630\n"
    ".ent func_00234630\n"
    "func_00234630:\n"
    "addiu $3, $0, 3\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234630\n"
    ".set reorder\n"
);
