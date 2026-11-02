/* EE kernel syscall stub 123 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _ExecOSD\n"
    ".ent _ExecOSD\n"
    "_ExecOSD:\n"
    "addiu $3, $0, 123\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _ExecOSD\n"
    ".set reorder\n"
);
