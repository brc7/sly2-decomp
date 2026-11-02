/* EE kernel syscall stub 1 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234610\n"
    ".ent func_00234610\n"
    "func_00234610:\n"
    "addiu $3, $0, 1\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234610\n"
    ".set reorder\n"
);
