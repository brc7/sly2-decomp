/* EE kernel syscall stub 127 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl GetMemorySize\n"
    ".ent GetMemorySize\n"
    "GetMemorySize:\n"
    "addiu $3, $0, 127\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end GetMemorySize\n"
    ".set reorder\n"
);
