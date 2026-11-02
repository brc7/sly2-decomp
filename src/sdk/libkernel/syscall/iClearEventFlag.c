/* EE kernel syscall stub -85 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iClearEventFlag\n"
    ".ent iClearEventFlag\n"
    "iClearEventFlag:\n"
    "addiu $3, $0, -85\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iClearEventFlag\n"
    ".set reorder\n"
);
