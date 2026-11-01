/* crt0 fragment after _start (hand-written assembly in the original crt0, like _start). */
/** @sly2-annot addr=0x100218
 * Purpose: UNKNOWN - no analysis recorded yet (byte-matched only).
 * Info: TU crt0_splicewrap
 * Confidence: none - unanalysed; anything inferred from the code here is a guess
 * @end-sly2-annot */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".text\n"
    ".globl func_00100218\n"
    ".ent func_00100218\n"
    "func_00100218:\n"
    "j func_002398D0\n"
    "nop\n"
    ".end func_00100218\n"
    ".set at\n"
    ".set reorder\n"
);
