/* EE kernel syscall stub 50 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SleepThread\n"
    ".ent SleepThread\n"
    "SleepThread:\n"
    "addiu $3, $0, 50\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SleepThread\n"
    ".set reorder\n"
);
