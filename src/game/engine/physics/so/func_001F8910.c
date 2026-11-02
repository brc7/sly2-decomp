struct E { int k; int pad[2]; int nC; struct E *next; };
struct Q { char pad[0x90]; struct E *pe; };
struct P { char pad[0x20]; void *p20; struct Q *pq; };
void func_00230100(void *, struct Q *, struct E *);
void func_0012E210(struct E *);
void func_002301D0(void *, struct Q *);
void func_002303A0(void *, struct Q *, int);
void SlotUnsettle(void *, struct Q *);
/** @sly2-annot addr=0x1F8910
 * Purpose: Find XF actuator of kind k; remove if n==-1, else update and re-prioritize
 * Info: proposed name PactFindXfh (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct E *func_001F8910(struct P *p, int k, int n, int m)
{
    struct E *pe;
    for (pe = p->pq->pe; pe; pe = pe->next) {
        if (pe->k != k) continue;
        if (n == -1) {
            func_00230100(p->p20, p->pq, pe);
            func_0012E210(pe);
            return 0;
        }
        if (m != pe->nC) {
            pe->nC = m;
            func_002301D0(p->p20, p->pq);
        }
        if (pe == p->pq->pe) {
            func_002303A0(p->p20, p->pq, n);
            SlotUnsettle(p->p20, p->pq);
        }
        return pe;
    }
    return 0;
}
