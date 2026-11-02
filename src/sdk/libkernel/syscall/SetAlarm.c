/* EE kernel syscall stub 252 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetAlarm\n"
    ".ent SetAlarm\n"
    "SetAlarm:\n"
    "addiu $3, $0, 252\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetAlarm\n"
    ".set reorder\n"
);
