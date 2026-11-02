/* EE kernel syscall stub 86 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_0023B358\n"
    ".ent func_0023B358\n"
    "func_0023B358:\n"
    "addiu $3, $0, 86\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_0023B358\n"
    ".set reorder\n"
);
