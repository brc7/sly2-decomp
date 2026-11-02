/* EE kernel syscall stub -56 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _iSuspendThread\n"
    ".ent _iSuspendThread\n"
    "_iSuspendThread:\n"
    "addiu $3, $0, -56\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _iSuspendThread\n"
    ".set reorder\n"
);
