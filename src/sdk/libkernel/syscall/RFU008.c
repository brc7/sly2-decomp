/* EE kernel syscall stub 8 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl RFU008\n"
    ".ent RFU008\n"
    "RFU008:\n"
    "addiu $3, $0, 8\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end RFU008\n"
    ".set reorder\n"
);
