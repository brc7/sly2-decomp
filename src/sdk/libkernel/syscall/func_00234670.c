/* EE kernel syscall stub 7 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234670\n"
    ".ent func_00234670\n"
    "func_00234670:\n"
    "addiu $3, $0, 7\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234670\n"
    ".set reorder\n"
);
