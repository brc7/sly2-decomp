/* EE kernel syscall stub 63 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl RFU063\n"
    ".ent RFU063\n"
    "RFU063:\n"
    "addiu $3, $0, 63\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end RFU063\n"
    ".set reorder\n"
);
