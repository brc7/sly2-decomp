/* EE kernel syscall stub 22 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234780\n"
    ".ent func_00234780\n"
    "func_00234780:\n"
    "addiu $3, $0, 22\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234780\n"
    ".set reorder\n"
);
