/* EE kernel syscall stub -254 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234800\n"
    ".ent func_00234800\n"
    "func_00234800:\n"
    "addiu $3, $0, -254\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234800\n"
    ".set reorder\n"
);
