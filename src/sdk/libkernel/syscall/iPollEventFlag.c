/* EE kernel syscall stub -88 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iPollEventFlag\n"
    ".ent iPollEventFlag\n"
    "iPollEventFlag:\n"
    "addiu $3, $0, -88\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iPollEventFlag\n"
    ".set reorder\n"
);
