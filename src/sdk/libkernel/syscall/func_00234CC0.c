/* EE kernel syscall stub -103 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234CC0\n"
    ".ent func_00234CC0\n"
    "func_00234CC0:\n"
    "addiu $3, $0, -103\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234CC0\n"
    ".set reorder\n"
);
