/* EE kernel syscall stub 127 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00234E80\n"
    ".ent func_00234E80\n"
    "func_00234E80:\n"
    "addiu $3, $0, 127\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00234E80\n"
    ".set reorder\n"
);
