/* EE kernel syscall stub 97 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl EnableCache\n"
    ".ent EnableCache\n"
    "EnableCache:\n"
    "addiu $3, $0, 97\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end EnableCache\n"
    ".set reorder\n"
);
