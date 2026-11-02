/* EE kernel syscall stub 128 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl func_00240C28\n"
    ".ent func_00240C28\n"
    "func_00240C28:\n"
    "addiu $3, $0, 128\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end func_00240C28\n"
    ".set reorder\n"
);
