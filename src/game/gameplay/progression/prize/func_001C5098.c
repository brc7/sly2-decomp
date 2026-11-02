struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct A { int c; void *pv; int pad[14]; };
struct M { int x0; int x4; int x8; };
struct S { char pad[0xC]; struct VTE *vt; char p1[0x168 - 0x10]; int x168; char p2[0x218 - 0x16C]; struct LO *x218; };
void HandleBasicMessage(struct S *p, int n, struct M *pm);
/** @sly2-annot addr=0x1C5098
 * Purpose: DPRIZE vt5 HandleMessage: base 0x133590 + prize-specific msgs
 * Info: proposed name HandleDprizeMessage (T1); class DPRIZE slot5; TU prize
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C5098(struct S *p, int n, struct M *pm)
{
    struct A a;
    HandleBasicMessage(p, n, pm);
    if (n == 10 && pm->x0 == p->x168 && pm->x8 == 0x186) {
        struct LO *plo = p->x218;
        if (plo) {
            a.c = 1;
            a.pv = p;
            ((void (*)(void *, struct A *))plo->vt[51].pf)((char *)plo + plo->vt[51].d, &a);
        }
        ((void (*)(void *, int))p->vt[76].pf)((char *)p + p->vt[76].d, 2);
    }
}
