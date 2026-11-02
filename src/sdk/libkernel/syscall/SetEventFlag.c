/* EE kernel syscall stub 82 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetEventFlag\n"
    ".ent SetEventFlag\n"
    "SetEventFlag:\n"
    "addiu $3, $0, 82\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetEventFlag\n"
    ".set reorder\n"
);
