struct S { char pad[0x204]; int x204; char p1[0x16C0]; int n; };
int func_00143C78(struct S *p);
/** @sly2-annot addr=0x18DEE0
 * Purpose: JT vt85: 0 if x204 or jts 3/8/21/22; 1 for 13/14; 0xF for 24; else base 0x143C78
 * Info: proposed name JT_vt85 (T2); class JT slot85; TU jt
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
int func_0018DEE0(struct S *p)
{
    if (p->x204) return 0;
    switch (p->n) {
    case 3: case 8: case 21: case 22:
        return 0;
    case 13: case 14:
        return 1;
    case 24:
        return 0xF;
    }
    return func_00143C78(p);
}
