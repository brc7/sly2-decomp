/* EE kernel syscall stub 131 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl FindAddress\n"
    ".ent FindAddress\n"
    "FindAddress:\n"
    "addiu $3, $0, 131\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end FindAddress\n"
    ".set reorder\n"
);
