void func_00205EB8(void *p);
extern void *D_002DD648;
/** @sly2-annot addr=0x14B1A0
 * Purpose: cid 0x3F (STEPGUARD child) vt7: STEPGUARD init, register as D_002DD648, +0x4A4=1
 * Info: proposed name InitCarmelita (T2); class CARMELITA slot7; TU tu_14A008
 * Notes: Cid3F->Cid63 (CID 0x3F=63, hex-literal) | class identity from whole_game WE names/object strings:
 *   InitCid63 -> InitCarmelita
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest+whole_game(class identity)
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
void func_0014B1A0(char *p)
{
    func_00205EB8(p);
    D_002DD648 = p;
    *(int *)(p + 0x4A4) = 1;
}
