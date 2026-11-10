/* EE kernel syscall stub 91 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl GetEntryAddress_00239958\n"
    ".ent GetEntryAddress_00239958\n"
    "GetEntryAddress_00239958:\n"
    "addiu $3, $0, 91\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end GetEntryAddress_00239958\n"
    ".set reorder\n"
);
