/* EE kernel syscall stub 35 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234850\n"
    ".ent func_00234850\n"
    "func_00234850:\n"
    "addiu $3, $0, 35\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234850\n"
    ".set reorder\n"
);
