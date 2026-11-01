struct Q { char pad[0x4C]; int n; };
struct P { char pad[0x40]; int fl; char pad2[0x17C - 0x44]; struct Q *px; };
void *func_001E7840(struct P *p, int f);
void SetLoBusyk(struct P *p, int n);
/** @sly2-annot addr=0x1E7A28
 * Purpose: n!=-1: ensure chain and enable updates (LO flags); -1: disable; store mode in chain +0x4C
 * Info: proposed name SetShapeChainMode (T1); TU shape
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6b_rest+verdicts_followups
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void SetShapeChainMode(struct P *p, int n)
{
    if (n != -1) {
        func_001E7840(p, 1);
        SetLoBusyk(p, 0);
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v &= ~0x20; else p->fl = v & ~0x20; }
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v |= 0xC; else p->fl = v | 0xC; }
    } else {
        SetLoBusyk(p, 0);
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v |= 0x20; else p->fl = v | 0x20; }
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v &= ~0xC; else p->fl = v & ~0xC; }
    }
    if (p->px) p->px->n = n;
}
