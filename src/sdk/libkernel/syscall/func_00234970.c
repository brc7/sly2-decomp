/* EE kernel syscall stub 53 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234970\n"
    ".ent func_00234970\n"
    "func_00234970:\n"
    "addiu $3, $0, 53\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234970\n"
    ".set reorder\n"
);
