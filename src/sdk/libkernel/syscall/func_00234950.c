/* EE kernel syscall stub 51 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234950\n"
    ".ent func_00234950\n"
    "func_00234950:\n"
    "addiu $3, $0, 51\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234950\n"
    ".set reorder\n"
);
