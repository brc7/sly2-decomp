/* EE kernel syscall stub 48 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl ReferThreadStatus\n"
    ".ent ReferThreadStatus\n"
    "ReferThreadStatus:\n"
    "addiu $3, $0, 48\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end ReferThreadStatus\n"
    ".set reorder\n"
);
