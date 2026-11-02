/* EE kernel syscall stub -90 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iReferEventFlagStatus\n"
    ".ent iReferEventFlagStatus\n"
    "iReferEventFlagStatus:\n"
    "addiu $3, $0, -90\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iReferEventFlagStatus\n"
    ".set reorder\n"
);
