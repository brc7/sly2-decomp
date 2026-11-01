struct ALLOCREQ { int cb; int nKind; };
class FRAME {
public:
    int a[4];
    FRAME();
    ~FRAME();
};
struct X { char pad[0x54]; int n; char pad2[0xA0 - 0x58]; int fA0; };
struct G { char pad[0x3AFC]; int c; };
extern G D_003DD490;
extern "C" void *func_001AE378(ALLOCREQ *preq);
extern "C" int func_0015E9C8(G *pg, int kind, int n, int m, int c, void **apv);
static inline void *PvAllocReq(ALLOCREQ *preq, int cb, int nKind)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return func_001AE378(preq);
}
static inline int Min(int a, int b) { return a > b ? b : a; }
/** @sly2-annot addr=0x161660
 * Purpose: Episode completion percent: over all TASKs (cid 0x85) with successors (cA0!=0), 100*complete(state
 *   3)/count, clamped to 1..99 unless 0 or all
 * Info: proposed name NEpisodePercentComplete (T1); TU game
 * Notes: runtime6 G14: percent formula recomputed from RAM (2 -> 7)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources: r3b_rest+runtime
 * See: docs/engine/game-progression.md#-complete, docs/engine/game-progression.md#8-key-functions,
 *   docs/engine/save-memcard.md#1-the-save-blob-g_gs-savecpp-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
extern "C" int func_00161660()
{
    int c = 0;
    int cDone = 0;
    FRAME fr;
    ALLOCREQ req;
    G *pg = &D_003DD490;
    X **apx = (X **)PvAllocReq(&req, pg->c * 4, 0x10);
    int n = func_0015E9C8(pg, 5, 0x85, 0, pg->c, (void **)apx);
    for (int i = 0; i < n; i++) {
        X *px = apx[i];
        if (px->fA0) {
            c++;
            if (px->n == 3)
                cDone++;
        }
    }
    if (!cDone)
        return 0;
    if (cDone == c)
        return 100;
    int nMax = 99; int pct = cDone * 100 / c; int nRet = 1; if (pct > 0) nRet = Min(pct, nMax); return nRet;
}
