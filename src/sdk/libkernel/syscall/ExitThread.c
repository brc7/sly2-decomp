/* EE kernel syscall stub 35 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl ExitThread\n"
    ".ent ExitThread\n"
    "ExitThread:\n"
    "addiu $3, $0, 35\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end ExitThread\n"
    ".set reorder\n"
);
