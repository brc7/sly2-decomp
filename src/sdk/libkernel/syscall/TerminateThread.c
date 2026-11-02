/* EE kernel syscall stub 37 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl TerminateThread\n"
    ".ent TerminateThread\n"
    "TerminateThread:\n"
    "addiu $3, $0, 37\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end TerminateThread\n"
    ".set reorder\n"
);
