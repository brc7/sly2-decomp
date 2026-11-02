/* EE kernel syscall stub 109 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234D10\n"
    ".ent func_00234D10\n"
    "func_00234D10:\n"
    "addiu $3, $0, 109\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234D10\n"
    ".set reorder\n"
);
