/* EE kernel syscall stub 125 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234E60\n"
    ".ent func_00234E60\n"
    "func_00234E60:\n"
    "addiu $3, $0, 125\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234E60\n"
    ".set reorder\n"
);
