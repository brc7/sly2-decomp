/* EE kernel syscall stub 39 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl DisableDispatchThread\n"
    ".ent DisableDispatchThread\n"
    "DisableDispatchThread:\n"
    "addiu $3, $0, 39\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end DisableDispatchThread\n"
    ".set reorder\n"
);
