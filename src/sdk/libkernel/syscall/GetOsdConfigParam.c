/* EE kernel syscall stub 75 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl GetOsdConfigParam\n"
    ".ent GetOsdConfigParam\n"
    "GetOsdConfigParam:\n"
    "addiu $3, $0, 75\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end GetOsdConfigParam\n"
    ".set reorder\n"
);
