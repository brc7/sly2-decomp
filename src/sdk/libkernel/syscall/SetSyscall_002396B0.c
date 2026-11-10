/* EE kernel syscall stub 116 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetSyscall_002396B0\n"
    ".ent SetSyscall_002396B0\n"
    "SetSyscall_002396B0:\n"
    "addiu $3, $0, 116\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetSyscall_002396B0\n"
    ".set reorder\n"
);
