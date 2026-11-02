/* EE kernel syscall stub 90 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00239728\n"
    ".ent func_00239728\n"
    "func_00239728:\n"
    "addiu $3, $0, 90\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00239728\n"
    ".set reorder\n"
);
