/* EE kernel syscall stub 68 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl WaitSema\n"
    ".ent WaitSema\n"
    "WaitSema:\n"
    "addiu $3, $0, 68\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end WaitSema\n"
    ".set reorder\n"
);
