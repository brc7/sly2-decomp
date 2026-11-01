typedef struct K { short x; short y; union { short s; int i; } u; } K;
typedef struct N { int a; K k; int d; struct N *pnNext; } N;
struct P { char pad[0x3C90]; char pool[0x18]; N *pnHead; N *pnTail; };
void func_0011CD98(void *pool, N *pn);
static __inline__ int FMatchK(K *pa, K *pb)
{
    if (pa->y != pb->y) return 0;
    if (pa->y != -1 && pa->u.s == pb->u.s) return 1;
    return pa->u.i == pb->u.i;
}
/** @sly2-annot addr=0x149B58
 * Purpose: Remove queued callbacks matching by grf bits: 1 = object, 2 = member pointer, 4 = arg; fix tail
 * Info: proposed name ClearSwCallbacks (T1); TU callback
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/world-sw.md#6-deferred-callbacks-and-the-splice-schedule,
 *   docs/engine/world-sw.md#10-key-functions, docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void func_00149B58(struct P *p, unsigned int grf, int a, K k, int d)
{
    N **ppn = &p->pnHead;
    N *pn;
    while ((pn = *ppn) != 0) {
        if ((grf & 1) && a != pn->a) goto next;
        if (grf & 2) {
            if (k.y != pn->k.y) goto next;
            if (k.y == -1 || k.u.s != pn->k.u.s) {
                if (k.u.i != pn->k.u.i) goto next;
            }
        }
        if ((grf & 4) && d != pn->d) goto next;
        *ppn = pn->pnNext;
        func_0011CD98(p->pool, pn);
        continue;
    next:
        ppn = &pn->pnNext;
    }
    if (!p->pnHead) p->pnTail = 0;
}
