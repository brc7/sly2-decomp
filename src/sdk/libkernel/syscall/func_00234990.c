/* EE kernel syscall stub 55 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234990\n"
    ".ent func_00234990\n"
    "func_00234990:\n"
    "addiu $3, $0, 55\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234990\n"
    ".set reorder\n"
);
