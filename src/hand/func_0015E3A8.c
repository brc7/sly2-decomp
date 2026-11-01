struct E { int a, b, c; };
/** @sly2-annot addr=0x15E3A8
 * Purpose: Returns the per-class object DL of the SW (SW+0x3038 + 8*cid)
 * Info: proposed name PdlFromSwCid (T1); TU find
 * Notes: runtime: per-class SL at SW+0x3038 + 12*cid (stride 12, SL {first,last,ibNext}; NOT 8) | runtime:
 *   per-class list stride at SW+0x3038 is 12 bytes
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources:
 *   r3_1539A8+runtime+runtime-corrected
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#find-0x15e380-0x15ef00
 * @end-sly2-annot */
struct E *func_0015E3A8(char *p, int i)
{
    return &((struct E *)(p + 0x3038))[i];
}
