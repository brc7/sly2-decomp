/* EE kernel syscall stub 59 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl JoinThread\n"
    ".ent JoinThread\n"
    "JoinThread:\n"
    "addiu $3, $0, 59\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end JoinThread\n"
    ".set reorder\n"
);
