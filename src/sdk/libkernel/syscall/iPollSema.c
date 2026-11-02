/* EE kernel syscall stub -70 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iPollSema\n"
    ".ent iPollSema\n"
    "iPollSema:\n"
    "addiu $3, $0, -70\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iPollSema\n"
    ".set reorder\n"
);
