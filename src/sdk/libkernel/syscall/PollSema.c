/* EE kernel syscall stub 69 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl PollSema\n"
    ".ent PollSema\n"
    "PollSema:\n"
    "addiu $3, $0, 69\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end PollSema\n"
    ".set reorder\n"
);
