/* EE kernel syscall stub 128 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _GetGsDxDyOffset\n"
    ".ent _GetGsDxDyOffset\n"
    "_GetGsDxDyOffset:\n"
    "addiu $3, $0, 128\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _GetGsDxDyOffset\n"
    ".set reorder\n"
);
