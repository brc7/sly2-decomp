/* EE kernel syscall stub -38 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iTerminateThread\n"
    ".ent iTerminateThread\n"
    "iTerminateThread:\n"
    "addiu $3, $0, -38\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iTerminateThread\n"
    ".set reorder\n"
);
