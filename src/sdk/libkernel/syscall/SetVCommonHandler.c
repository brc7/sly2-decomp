/* EE kernel syscall stub 14 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetVCommonHandler\n"
    ".ent SetVCommonHandler\n"
    "SetVCommonHandler:\n"
    "addiu $3, $0, 14\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetVCommonHandler\n"
    ".set reorder\n"
);
