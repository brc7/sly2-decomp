/* EE kernel syscall stub 66 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SignalSema\n"
    ".ent SignalSema\n"
    "SignalSema:\n"
    "addiu $3, $0, 66\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SignalSema\n"
    ".set reorder\n"
);
