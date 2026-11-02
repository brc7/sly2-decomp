/* EE kernel syscall stub 40 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl EnableDispatchThread\n"
    ".ent EnableDispatchThread\n"
    "EnableDispatchThread:\n"
    "addiu $3, $0, 40\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end EnableDispatchThread\n"
    ".set reorder\n"
);
