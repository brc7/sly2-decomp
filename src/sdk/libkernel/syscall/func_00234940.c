/* EE kernel syscall stub 50 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234940\n"
    ".ent func_00234940\n"
    "func_00234940:\n"
    "addiu $3, $0, 50\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234940\n"
    ".set reorder\n"
);
