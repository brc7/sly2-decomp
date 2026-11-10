/* EE kernel syscall stub 124 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl Deci2Call\n"
    ".ent Deci2Call\n"
    "Deci2Call:\n"
    "addiu $3, $0, 124\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end Deci2Call\n"
    ".set reorder\n"
);
