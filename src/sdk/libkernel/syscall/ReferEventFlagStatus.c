/* EE kernel syscall stub 89 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl ReferEventFlagStatus\n"
    ".ent ReferEventFlagStatus\n"
    "ReferEventFlagStatus:\n"
    "addiu $3, $0, 89\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end ReferEventFlagStatus\n"
    ".set reorder\n"
);
