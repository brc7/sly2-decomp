/* EE kernel syscall stub 60 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetupThread\n"
    ".ent SetupThread\n"
    "SetupThread:\n"
    "addiu $3, $0, 60\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetupThread\n"
    ".set reorder\n"
);
