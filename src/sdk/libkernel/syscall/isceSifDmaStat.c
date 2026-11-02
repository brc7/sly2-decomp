/* EE kernel syscall stub -118 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl isceSifDmaStat\n"
    ".ent isceSifDmaStat\n"
    "isceSifDmaStat:\n"
    "addiu $3, $0, -118\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end isceSifDmaStat\n"
    ".set reorder\n"
);
