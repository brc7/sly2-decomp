/* EE kernel syscall stub 253 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl ReleaseAlarm\n"
    ".ent ReleaseAlarm\n"
    "ReleaseAlarm:\n"
    "addiu $3, $0, 253\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end ReleaseAlarm\n"
    ".set reorder\n"
);
