/* EE kernel syscall stub -254 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iSetAlarm\n"
    ".ent iSetAlarm\n"
    "iSetAlarm:\n"
    "addiu $3, $0, -254\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iSetAlarm\n"
    ".set reorder\n"
);
