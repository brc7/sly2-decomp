/* EE kernel syscall stub 130 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _InitTLB\n"
    ".ent _InitTLB\n"
    "_InitTLB:\n"
    "addiu $3, $0, 130\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _InitTLB\n"
    ".set reorder\n"
);
