/* EE kernel syscall stub 111 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234D30\n"
    ".ent func_00234D30\n"
    "func_00234D30:\n"
    "addiu $3, $0, 111\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234D30\n"
    ".set reorder\n"
);
