/* EE kernel syscall stub -54 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iCancelWakeupThread\n"
    ".ent iCancelWakeupThread\n"
    "iCancelWakeupThread:\n"
    "addiu $3, $0, -54\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iCancelWakeupThread\n"
    ".set reorder\n"
);
