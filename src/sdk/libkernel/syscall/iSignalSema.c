/* EE kernel syscall stub -67 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iSignalSema\n"
    ".ent iSignalSema\n"
    "iSignalSema:\n"
    "addiu $3, $0, -67\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iSignalSema\n"
    ".set reorder\n"
);
