void func_001EEF70(void *p);
void func_0014AE38(void *p, int n);
extern float D_002DDED0;
/** @sly2-annot addr=0x14A200
 * Purpose: BHV182 slot 8: SetBhv182State(0), timestamp, guard +0x4A4 = 0
 * Info: proposed name StartBhv182 (T2); class BHV182 slot8; TU tu_14A008
 * Notes: superseded: OnBhv182Resume (r2b_rest) | BHV vtable slot 8 role per r9 (CONFLICTS #15); was
 *   OnBhv182Resume
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
void func_0014A200(char *p)
{
    func_001EEF70(p);
    func_0014AE38(p, 0);
    *(float *)(p + 0x48) = D_002DDED0;
    *(int *)(*(char **)(p + 0x10) + 0x4A4) = 0;
}
