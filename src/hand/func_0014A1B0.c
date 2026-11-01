void func_001EEF68(void *p);
void func_001FEE90(void *p);
void func_00209890(void *p, int n);
extern char D_003DD040[];
/** @sly2-annot addr=0x14A1B0
 * Purpose: BHV182 slot 7: base, start music/stream cue (0x1FEE90 on D_003DD040), guard flag 0x24, indices -1
 * Info: proposed name InitBhv182 (T2); class BHV182 slot7; TU tu_14A008
 * Notes: superseded: OnBhv182Push (r2b_rest) | BHV vtable slot 7 role per r9 (CONFLICTS #15); was
 *   OnBhv182Push
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
void func_0014A1B0(char *p)
{
    func_001EEF68(p);
    func_001FEE90(D_003DD040);
    func_00209890(*(void **)(p + 0x10), 0x24);
    *(int *)(p + 0x30) = -1;
    *(int *)(p + 0x40) = -1;
    *(int *)(p + 0x3C) = -1;
}
