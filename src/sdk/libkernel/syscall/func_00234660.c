/* EE kernel syscall stub 6 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234660\n"
    ".ent func_00234660\n"
    "func_00234660:\n"
    "addiu $3, $0, 6\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234660\n"
    ".set reorder\n"
);
