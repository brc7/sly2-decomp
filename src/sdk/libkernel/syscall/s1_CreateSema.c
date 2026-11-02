/* EE kernel syscall stub 64 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl s1_CreateSema\n"
    ".ent s1_CreateSema\n"
    "s1_CreateSema:\n"
    "addiu $3, $0, 64\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end s1_CreateSema\n"
    ".set reorder\n"
);
