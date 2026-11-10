/* EE kernel syscall stub 55 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SuspendThread\n"
    ".ent SuspendThread\n"
    "SuspendThread:\n"
    "addiu $3, $0, 55\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SuspendThread\n"
    ".set reorder\n"
);
