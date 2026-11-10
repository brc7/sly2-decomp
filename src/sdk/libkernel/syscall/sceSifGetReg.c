/* EE kernel syscall stub 122 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl sceSifGetReg\n"
    ".ent sceSifGetReg\n"
    "sceSifGetReg:\n"
    "addiu $3, $0, 122\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end sceSifGetReg\n"
    ".set reorder\n"
);
