/* EE kernel syscall stub 20 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234760\n"
    ".ent func_00234760\n"
    "func_00234760:\n"
    "addiu $3, $0, 20\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234760\n"
    ".set reorder\n"
);
