/* EE kernel syscall stub 47 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234910\n"
    ".ent func_00234910\n"
    "func_00234910:\n"
    "addiu $3, $0, 47\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234910\n"
    ".set reorder\n"
);
