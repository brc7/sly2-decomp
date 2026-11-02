/* EE kernel syscall stub 91 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl GetEntryAddress\n"
    ".ent GetEntryAddress\n"
    "GetEntryAddress:\n"
    "addiu $3, $0, 91\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end GetEntryAddress\n"
    ".set reorder\n"
);
