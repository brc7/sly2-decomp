/* EE kernel syscall stub 51 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl WakeupThread\n"
    ".ent WakeupThread\n"
    "WakeupThread:\n"
    "addiu $3, $0, 51\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end WakeupThread\n"
    ".set reorder\n"
);
