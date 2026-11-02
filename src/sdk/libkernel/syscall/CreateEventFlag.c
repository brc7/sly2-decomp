/* EE kernel syscall stub 80 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl CreateEventFlag\n"
    ".ent CreateEventFlag\n"
    "CreateEventFlag:\n"
    "addiu $3, $0, 80\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end CreateEventFlag\n"
    ".set reorder\n"
);
