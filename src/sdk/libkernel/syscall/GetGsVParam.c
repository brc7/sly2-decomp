/* EE kernel syscall stub 77 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl GetGsVParam\n"
    ".ent GetGsVParam\n"
    "GetGsVParam:\n"
    "addiu $3, $0, 77\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end GetGsVParam\n"
    ".set reorder\n"
);
