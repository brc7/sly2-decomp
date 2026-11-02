/* EE kernel syscall stub -52 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234960\n"
    ".ent func_00234960\n"
    "func_00234960:\n"
    "addiu $3, $0, -52\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234960\n"
    ".set reorder\n"
);
