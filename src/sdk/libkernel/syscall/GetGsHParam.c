/* EE kernel syscall stub 76 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl GetGsHParam\n"
    ".ent GetGsHParam\n"
    "GetGsHParam:\n"
    "addiu $3, $0, 76\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end GetGsHParam\n"
    ".set reorder\n"
);
