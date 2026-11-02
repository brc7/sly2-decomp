/* EE kernel syscall stub 84 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl ClearEventFlag\n"
    ".ent ClearEventFlag\n"
    "ClearEventFlag:\n"
    "addiu $3, $0, 84\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end ClearEventFlag\n"
    ".set reorder\n"
);
