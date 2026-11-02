/* EE kernel syscall stub 5 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl RFU005\n"
    ".ent RFU005\n"
    "RFU005:\n"
    "addiu $3, $0, 5\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end RFU005\n"
    ".set reorder\n"
);
