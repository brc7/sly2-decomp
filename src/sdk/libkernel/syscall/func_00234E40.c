/* EE kernel syscall stub 123 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234E40\n"
    ".ent func_00234E40\n"
    "func_00234E40:\n"
    "addiu $3, $0, 123\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234E40\n"
    ".set reorder\n"
);
