/* crt0 fragment: ExitThread syscall tail (hand-written assembly in the original crt0, like _start). */
/** @sly2-annot addr=0x100220
 * Purpose: UNKNOWN - no analysis recorded yet (byte-matched only).
 * Info: TU crt0_splicewrap
 * Confidence: none - unanalysed; anything inferred from the code here is a guess
 * @end-sly2-annot */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".text\n"
    ".globl func_00100220\n"
    ".ent func_00100220\n"
    "func_00100220:\n"
    "addiu $3, $0, 0x23\n"
    "syscall 0\n"
    ".end func_00100220\n"
    ".set at\n"
    ".set reorder\n"
);
