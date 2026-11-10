/* EE kernel syscall stub -120 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl isceSifSetDChain\n"
    ".ent isceSifSetDChain\n"
    "isceSifSetDChain:\n"
    "addiu $3, $0, -120\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end isceSifSetDChain\n"
    ".set reorder\n"
);
