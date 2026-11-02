/* EE kernel syscall stub 126 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl MachineType\n"
    ".ent MachineType\n"
    "MachineType:\n"
    "addiu $3, $0, 126\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end MachineType\n"
    ".set reorder\n"
);
