struct O { char pad[0xB8]; int nB8; char p2[0xC]; int nC8; };
struct S { char pad[0x3E5C]; int n; };
int func_0015E9C8(struct S *p, int a, int b, int c, int cMax, struct O **apo);
void func_001A6D28(struct O *o, int n);
/** @sly2-annot addr=0x20E2B0
 * Purpose: Find up to 16 objects of class MS (0x31); keep the one with largest +0xB8, store its +0xC8 and
 *   call 0x1A6D28(po,0xFF)
 * Info: proposed name BindSwMs (T2); TU sw
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020E2B0(struct S *p)
{
    struct O *apo[16];
    int c = func_0015E9C8(p, 5, 0x31, 0, 16, apo);
    int nMax = 0;
    struct O *poBest = 0;
    int i;
    for (i = 0; i < c; i++) {
        if (apo[i]->nB8 > nMax) {
            nMax = apo[i]->nB8;
            poBest = apo[i];
        }
    }
    if (poBest) {
        p->n = poBest->nC8;
        func_001A6D28(poBest, 0xFF);
    }
}
