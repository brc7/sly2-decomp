/* EE kernel syscall stub 7 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _ExecPS2\n"
    ".ent _ExecPS2\n"
    "_ExecPS2:\n"
    "addiu $3, $0, 7\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _ExecPS2\n"
    ".set reorder\n"
);
