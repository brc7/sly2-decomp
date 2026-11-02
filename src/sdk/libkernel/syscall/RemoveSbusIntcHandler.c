/* EE kernel syscall stub 11 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl RemoveSbusIntcHandler\n"
    ".ent RemoveSbusIntcHandler\n"
    "RemoveSbusIntcHandler:\n"
    "addiu $3, $0, 11\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end RemoveSbusIntcHandler\n"
    ".set reorder\n"
);
