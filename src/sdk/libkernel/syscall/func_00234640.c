/* EE kernel syscall stub 4 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234640\n"
    ".ent func_00234640\n"
    "func_00234640:\n"
    "addiu $3, $0, 4\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234640\n"
    ".set reorder\n"
);
