/* EE kernel syscall stub 81 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl DeleteEventFlag\n"
    ".ent DeleteEventFlag\n"
    "DeleteEventFlag:\n"
    "addiu $3, $0, 81\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end DeleteEventFlag\n"
    ".set reorder\n"
);
