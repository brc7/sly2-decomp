struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct N { struct LO *plo; int x4; struct N *pnNext; };
struct S { int x0; int cRef; char p1[0xE8 - 0x8]; struct N *pn; struct VTE *vt; };
extern char D_003E1144[];
extern char D_003E14E4[];
void func_0011CD98(void *pool, void *pv);
/** @sly2-annot addr=0x1CE9D0
 * Purpose: Decref; at 0 call OnRemove virtual, notify subscribed LOs (msg 8) and free subscription nodes,
 *   free rip
 * Info: proposed name ReleaseRip (T1); TU rip
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void ReleaseRip__FP3RIP(struct S *p)
{
    struct N *pn, *pnNext;
    if (--p->cRef == 0) {
        ((void (*)(void *))p->vt[3].pf)((char *)p + p->vt[3].d);
        for (pn = p->pn; pn; pn = pnNext) {
            struct LO *plo = pn->plo;
            ((void (*)(void *, int, struct S *))plo->vt[5].pf)((char *)plo + plo->vt[5].d, 8, p);
            pnNext = pn->pnNext;
            func_0011CD98(D_003E1144, pn);
        }
        p->pn = 0;
        func_0011CD98(D_003E14E4, p);
    }
}
