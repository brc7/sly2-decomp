/* EE kernel syscall stub -255 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iReleaseAlarm\n"
    ".ent iReleaseAlarm\n"
    "iReleaseAlarm:\n"
    "addiu $3, $0, -255\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iReleaseAlarm\n"
    ".set reorder\n"
);
