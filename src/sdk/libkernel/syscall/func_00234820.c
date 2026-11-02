/* EE kernel syscall stub 32 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234820\n"
    ".ent func_00234820\n"
    "func_00234820:\n"
    "addiu $3, $0, 32\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234820\n"
    ".set reorder\n"
);
