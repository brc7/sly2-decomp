struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { int a[3]; struct VTE *vt; char pad[0x1B8]; int n1c8; };
struct Q { char pad[0x44]; int grf; };
/** @sly2-annot addr=0x1C5DF0
 * Purpose: DPRIZE new virtual slot 75
 * Info: proposed name DPRIZE_VUnk75 (T2); class DPRIZE slot75; TU prize
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
int func_001C5DF0(struct O *p, struct Q *q)
{
    if (!p->n1c8) return 0;
    if (!(q->grf & 8)) return 0;
    { struct VTE *e = &p->vt[76]; e->pfn((char *)p + e->d, 1); }
    return 1;
}
