/* EE kernel syscall stub -28 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _iEnableDmac\n"
    ".ent _iEnableDmac\n"
    "_iEnableDmac:\n"
    "addiu $3, $0, -28\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _iEnableDmac\n"
    ".set reorder\n"
);
