/* EE kernel syscall stub -119 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl isceSifSetDma\n"
    ".ent isceSifSetDma\n"
    "isceSifSetDma:\n"
    "addiu $3, $0, -119\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end isceSifSetDma\n"
    ".set reorder\n"
);
