/* EE kernel syscall stub 86 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234B80\n"
    ".ent func_00234B80\n"
    "func_00234B80:\n"
    "addiu $3, $0, 86\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234B80\n"
    ".set reorder\n"
);
