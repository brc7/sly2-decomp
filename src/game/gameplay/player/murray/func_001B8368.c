struct S { char pad[0x204]; int f204; char pad2[0x13D0-0x208]; int n; int reserved; int m; };
int func_00143C78(struct S *p);
/** @sly2-annot addr=0x1B8368
 * Purpose: MURRAY vt85: 0 if x204 set, mode 1 or state 20, else base 0x143C78
 * Info: proposed name MURRAY_vt85 (T2); class MURRAY slot85; TU murray
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
int func_001B8368(struct S *p)
{
    if (p->f204) return 0;
    if (p->m == 1 || p->n == 20) return 0;
    return func_00143C78(p);
}
