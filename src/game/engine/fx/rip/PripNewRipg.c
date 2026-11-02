struct R { int a, b; void *p8; };
struct G { char pad[0x4054]; char pool[4]; };
struct Q { char pad[0x15C]; char dl[4]; };
extern char D_003DD490[];
struct Q *PripgNew(void *pg, int n);
void *PvAllocSlotheapClear(void *pool);
struct R *PvtripFromRipt(void *pv, void *a);
void AppendDlEntry__FP2DLPv(void *pdl, void *pv);
void func_001CE9C0(struct R *r);
/** @sly2-annot addr=0x1CE888
 * Purpose: Allocate a RIP from the rip pool (D_003E1144?), construct by type, link into RIPG dlRip, AddRipRef
 * Info: proposed name PripNewRipg (T1); TU rip
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5+verdicts_A2
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
struct R *PripNewRipg(struct G *p, void *a, struct Q *q)
{
    void *pv;
    struct R *r;
    if (!q) q = PripgNew(D_003DD490, 0);
    if (!q) return 0;
    pv = PvAllocSlotheapClear(p->pool);
    if (pv) {
    r = PvtripFromRipt(pv, a);
    r->p8 = q;
    AppendDlEntry__FP2DLPv(q->dl, r);
    func_001CE9C0(r);
    return r; }
    return 0;
}
