/* EE kernel syscall stub 86 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl WaitEventFlag\n"
    ".ent WaitEventFlag\n"
    "WaitEventFlag:\n"
    "addiu $3, $0, 86\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end WaitEventFlag\n"
    ".set reorder\n"
);
