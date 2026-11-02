/* EE kernel syscall stub 33 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl DeleteThread\n"
    ".ent DeleteThread\n"
    "DeleteThread:\n"
    "addiu $3, $0, 33\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end DeleteThread\n"
    ".set reorder\n"
);
