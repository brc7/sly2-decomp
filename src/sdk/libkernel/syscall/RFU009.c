/* EE kernel syscall stub 9 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl RFU009\n"
    ".ent RFU009\n"
    "RFU009:\n"
    "addiu $3, $0, 9\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end RFU009\n"
    ".set reorder\n"
);
