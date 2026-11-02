/* EE kernel syscall stub 16 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234710\n"
    ".ent func_00234710\n"
    "func_00234710:\n"
    "addiu $3, $0, 16\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234710\n"
    ".set reorder\n"
);
