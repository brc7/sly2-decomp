/* EE kernel syscall stub 32 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl CreateThread\n"
    ".ent CreateThread\n"
    "CreateThread:\n"
    "addiu $3, $0, 32\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end CreateThread\n"
    ".set reorder\n"
);
