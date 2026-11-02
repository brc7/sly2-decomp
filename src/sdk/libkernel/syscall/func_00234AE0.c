/* EE kernel syscall stub 76 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234AE0\n"
    ".ent func_00234AE0\n"
    "func_00234AE0:\n"
    "addiu $3, $0, 76\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234AE0\n"
    ".set reorder\n"
);
