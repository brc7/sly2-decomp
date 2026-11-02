/* EE kernel syscall stub -112 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234D50\n"
    ".ent func_00234D50\n"
    "func_00234D50:\n"
    "addiu $3, $0, -112\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234D50\n"
    ".set reorder\n"
);
