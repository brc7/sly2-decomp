typedef unsigned long ulong;
struct VT { short d; short pad; int (*pfn)(void *); };
struct O { char pad[0xC]; struct VT *pvt; };
struct S { char pad[8]; struct O *po; char pad2[0x9C - 0xC]; int k; char pad3[0xA8 - 0xA0]; int i; struct S *pNext; };
struct X;
struct L { struct S *ps; struct L *pNext; char pad[4]; struct X *px; };
struct E { char pad[0x64]; struct L *pl; struct S *ps; };
struct X { char pad[0xEC]; struct E *pe; char pad2[0x148 - 0xF0]; ulong fl; };

/** @sly2-annot addr=0x1F90D0
 * Purpose: Collect the contact points (XP) of a group of SOs into an array (max cMax), marking objects
 *   0x10000000; returns count
 * Info: proposed name CpxpBuildXpArray (T1); TU solve
 * Notes: names_v2 v1 proposed GetSoContacts__FP2SOPiPPP2SO (0.638)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
int func_001F90D0(int a, int b, int c, struct X **apx, int cMax, struct S **aps)
{
    int i;
    int cs = 0;
    for (i = 0; i < c; i++) {
        struct X *px = apx[i];
        struct L *pl;
        px->fl |= 0x10000000;
        pl = px->pe->pl;
        while (1) {
            struct S *ps;
        again:
            if (pl) {
                if (pl->px->fl & 0x10000000) { pl = pl->pNext; goto again; }
                ps = pl->ps;
            } else ps = px->pe->ps;
            for (; ps; ps = ps->pNext) {
                if (ps->k == 0xD) continue;
                if (ps->k == 0xE) {
                    struct O *po = ps->po;
                    if (!(po->pvt[85].pfn((char *)po + po->pvt[85].d) & 4)) continue;
                }
                if (cs >= cMax) return cs;
                ps->i = cs;
                aps[cs++] = ps;
            }
            if (!pl) break;
            pl = pl->pNext;
        }
    }
    return cs;
}
