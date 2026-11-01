typedef unsigned long ulong;
struct VTE { short d; short p; int (*pfn)(void *); };
struct E { char pad[0x10]; ulong fl; char pad2[0xB0 - 0x18]; };
struct P;
struct Q { char pad[0x34]; char dl[0x2C]; struct P *p60; };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0xE0 - 0x10]; int nE0; struct E *aE4; int nE8; struct Q *pq; };
struct Q2 { char pad[0xEC]; struct Q *pq; };
struct G;
extern struct G D_003DD490;
int func_0011A380(void *, void *);
void func_0011A2E8(void *, void *);
int func_0011A3B0(void *);
void func_001A00D0(struct G *, int, struct E *);
void func_001F4E28(struct P *);
void func_001337D0(struct G *, struct Q *);
void func_001F5078(struct P *);
/** @sly2-annot addr=0x1F2930
 * Purpose: Remove a SOPART from the tree DL; free its XP list; drop tree when empty (0x1337D0) or detach if
 *   no own parts remain
 * Info: proposed name RemoveSoPartFromTree (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F2930(struct P *p, struct E *pe)
{
    int i;
    if (!p->pq) return;
    if (!func_0011A380(p->pq->dl, pe)) return;
    func_0011A2E8(p->pq->dl, pe);
    if (pe->fl & (1L << 51)) func_001A00D0(&D_003DD490, 0, pe);
    func_001F4E28(p);
    if (func_0011A3B0(p->pq->dl)) {
        struct Q *pq = p->pq;
        ((struct Q2 *)pq->p60)->pq = 0;
        p->pq = 0;
        func_001337D0(&D_003DD490, pq);
    } else if (p->pq->p60 != p) {
        i = p->nE0;
        while (--i >= 0) {
            if (func_0011A380(p->pq->dl, (struct E *)(i * 0xB0 + (int)p->aE4))) break;
        }
        if (i < 0) p->pq = 0;
    }
done:
    if (!p->pvt[62].pfn((char *)p + p->pvt[62].d)) func_001F5078(p);
}
