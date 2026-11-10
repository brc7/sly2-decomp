/* EE kernel syscall stub 64 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl CreateSema\n"
    ".ent CreateSema\n"
    "CreateSema:\n"
    "addiu $3, $0, 64\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end CreateSema\n"
    ".set reorder\n"
);
