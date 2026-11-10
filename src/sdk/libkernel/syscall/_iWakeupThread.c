/* EE kernel syscall stub -52 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _iWakeupThread\n"
    ".ent _iWakeupThread\n"
    "_iWakeupThread:\n"
    "addiu $3, $0, -52\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _iWakeupThread\n"
    ".set reorder\n"
);
