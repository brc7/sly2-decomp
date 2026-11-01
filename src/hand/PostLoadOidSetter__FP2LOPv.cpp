class X;
typedef void (X::*PMF)(void *);
struct D { int id; PMF pmf; };
extern "C" char D_003DD490[];
extern "C" void *PloFindSwObject__FP2SWi3OIDP2LO(void *, int, int, X *);
/** @sly2-annot addr=0x13E5E0
 * Purpose: LORUN callback: finds object by oid (PloFindSwObject mode 4) and calls the stored
 *   pointer-to-member setter with it
 * Info: proposed name PostLoadOidSetter (T1); TU brx
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#brx-0x13e368-0x13ff70
 * @end-sly2-annot */
extern "C" void PostLoadOidSetter__FP2LOPv(X *p, D *pd)
{
    void *pv = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pd->id, p);
    (p->*(pd->pmf))(pv);
}
