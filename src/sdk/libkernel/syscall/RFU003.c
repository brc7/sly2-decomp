/* EE kernel syscall stub 3 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl RFU003\n"
    ".ent RFU003\n"
    "RFU003:\n"
    "addiu $3, $0, 3\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end RFU003\n"
    ".set reorder\n"
);
