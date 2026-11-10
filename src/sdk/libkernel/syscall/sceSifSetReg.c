/* EE kernel syscall stub 121 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl sceSifSetReg\n"
    ".ent sceSifSetReg\n"
    "sceSifSetReg:\n"
    "addiu $3, $0, 121\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end sceSifSetReg\n"
    ".set reorder\n"
);
