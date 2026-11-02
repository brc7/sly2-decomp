typedef unsigned long ulong;
struct O { char pad[0x148]; long bits; };
struct N { int x0; struct O *po; int x8; struct N *next; };
struct G;
extern struct G D_003DD490;
void func_00229738(struct O *po, struct N *pn);
void AddSoXa(struct O *po, struct N *pn);
void func_002296C0(struct G *pg, struct N *pn);
/** @sly2-annot addr=0x228D20
 * Purpose: rebuilds the per-object XA records (add/remove, free unused)
 * Info: proposed name UpdateWaterXaList (T2); TU water
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88+verdicts_B
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
void func_00228D20(void *pv, struct N **ppn)
{
    struct N *pn = *ppn;
    struct N *pnFree = 0;
    while (pn) {
        struct O *po = pn->po;
        struct N *pnNext = pn->next;
        long b = po->bits;
        if (((int)(b >> 31) & 1) != ((b >> 32) & 1)) {
            if (b & 0x80000000L)
                func_00229738(po, pn);
            else
                AddSoXa(po, pn);
        }
        if (pn->po->bits & 0x100000000L) {
            ppn = &pn->next;
        } else {
            *ppn = pnNext;
            pn->next = pnFree;
            pnFree = pn;
        }
        {
            long c = pn->po->bits & ~0x100000000L;
            pn->po->bits = c & ~0x80000000L;
        }
        pn = pnNext;
    }
    func_002296C0(&D_003DD490, pnFree);
}
