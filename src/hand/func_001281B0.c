// (Claude agent, 2026-10-17) MATCH. GC mark loop: pops frame / closure / node stacks of GC, marks children (bit31 @+4 vs
// D_002DD598) and pushes unmarked ones. Register keys: one function-level `pcRef` for every closure pointer (the tagk 0x22
// case marks through the stale pointer of an earlier 0x20 case - original behaviour), one `pf` for the popped frame and the
// closure's frame, one `pn` for the closure's link list walk, its node and the popped node.
typedef struct OBJ { int x0; unsigned n : 31; unsigned fMark : 1; } OBJ;
typedef struct CRef { int m_tagk; OBJ *pobj; } CRef;
typedef struct BIND { int x0; unsigned n : 31; unsigned fMark : 1; int x8; CRef ref; struct BIND *pbNext; } BIND;
typedef struct FRAME { int x0; unsigned n : 31; unsigned fMark : 1; int c; struct FRAME *apf[1]; } FRAME;
typedef struct FRAMEX { int x0; unsigned n : 31; unsigned fMark : 1; int c; int pad[4]; BIND *pbFirst; } FRAMEX;
typedef struct NODE { int x0; unsigned n : 31; unsigned fMark : 1; CRef car; struct NODE *pnNext; } NODE;
typedef struct LINK { int x0; unsigned n : 31; unsigned fMark : 1; int x8; int xC; struct LINK *plNext; } LINK;
typedef struct CLO { int x0; unsigned n : 31; unsigned fMark : 1; FRAME *pf; int xC; int x10; LINK *pl; NODE *pn; } CLO;
typedef struct GC { int f; int cFrame; FRAME **apFrame; int cNode; NODE **apNode; int cClo; CLO **apClo; } GC;
extern unsigned int D_002DD598[1];

static inline void PushNode(GC *pgc, NODE *pn) { pgc->apNode[pgc->cNode++] = pn; }
static inline void PushClo(GC *pgc, CLO *pc) { pgc->apClo[pgc->cClo++] = pc; }
static inline void PushFrame(GC *pgc, FRAME *pf) { pgc->apFrame[pgc->cFrame++] = pf; }

#define MARKREF(pref, STMT22) \
    switch ((pref)->m_tagk) { \
    case 0x1F: { \
        NODE *pn_ = (NODE *)(pref)->pobj; \
        if (pn_->fMark != D_002DD598[0]) { pn_->fMark = D_002DD598[0]; PushNode(pgc, pn_); } \
        break; } \
    case 0x20: \
        pcRef = (CLO *)(pref)->pobj; \
        if (pcRef->fMark != D_002DD598[0]) { pcRef->fMark = D_002DD598[0]; PushClo(pgc, pcRef); } \
        break; \
    case 0x22: STMT22; break; \
    }

/** @sly2-annot addr=0x1281B0
 * Purpose: Mark loop: pops frame/proc/pair stacks, marks children with current mark bit and pushes unmarked
 * Info: proposed name CGc_MarkLiveObjects (T1); class CGc; TU gc
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#gc-0x128138-0x128b60
 * @end-sly2-annot */
void func_001281B0(GC *pgc)
{
    CLO *pcRef;
    NODE *pn;
    FRAME *pf;
    for (;;) {
        pf = (pgc->cFrame > 0) ? pgc->apFrame[--pgc->cFrame] : 0;
        if (pf) {
            BIND *pb;
            int i;
            for (pb = ((FRAMEX *)pf)->pbFirst; pb; pb = pb->pbNext) {
                CRef *pref;
                pb->fMark = D_002DD598[0];
                pref = &pb->ref;
                MARKREF(pref, pcRef->fMark = D_002DD598[0])
            }
            for (i = 0; i < pf->c; i++) {
                FRAME *pfChild = pf->apf[i];
                if (pfChild->fMark != D_002DD598[0]) {
                    pfChild->fMark = D_002DD598[0];
                    PushFrame(pgc, pfChild);
                }
            }
            continue;
        }
        {
            pcRef = (pgc->cClo > 0) ? pgc->apClo[--pgc->cClo] : 0;
            if (pcRef) {
                for (pn = (NODE *)pcRef->pl; pn; pn = pn->pnNext) {
                    if (pn->fMark != D_002DD598[0])
                        pn->fMark = D_002DD598[0];
                }
                pn = pcRef->pn;
                if (pn->fMark != D_002DD598[0]) {
                    pn->fMark = D_002DD598[0];
                    PushNode(pgc, pn);
                }
                pf = pcRef->pf;
                if (pf->fMark != D_002DD598[0]) {
                    pf->fMark = D_002DD598[0];
                    PushFrame(pgc, pf);
                }
                continue;
            }
        }
        {
            NODE *pnNext;
            pn = (pgc->cNode > 0) ? pgc->apNode[--pgc->cNode] : 0;
            if (!pn)
                return;
            MARKREF(&pn->car, pn->car.pobj->fMark = D_002DD598[0])
            pnNext = pn->pnNext;
            if (pnNext && pnNext->fMark != D_002DD598[0]) {
                pnNext->fMark = D_002DD598[0];
                PushNode(pgc, pnNext);
            }
        }
    }
}
