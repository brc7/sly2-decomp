/* EE kernel syscall stub 117 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _print\n"
    ".ent _print\n"
    "_print:\n"
    "addiu $3, $0, 117\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _print\n"
    ".set reorder\n"
);
