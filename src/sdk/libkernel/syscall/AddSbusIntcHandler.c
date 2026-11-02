/* EE kernel syscall stub 10 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl AddSbusIntcHandler\n"
    ".ent AddSbusIntcHandler\n"
    "AddSbusIntcHandler:\n"
    "addiu $3, $0, 10\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end AddSbusIntcHandler\n"
    ".set reorder\n"
);
