/* EE kernel syscall stub -83 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iSetEventFlag\n"
    ".ent iSetEventFlag\n"
    "iSetEventFlag:\n"
    "addiu $3, $0, -83\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iSetEventFlag\n"
    ".set reorder\n"
);
