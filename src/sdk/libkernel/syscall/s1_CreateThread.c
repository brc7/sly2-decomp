/* EE kernel syscall stub -27 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl s1_CreateThread\n"
    ".ent s1_CreateThread\n"
    "s1_CreateThread:\n"
    "addiu $3, $0, -27\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end s1_CreateThread\n"
    ".set reorder\n"
);
