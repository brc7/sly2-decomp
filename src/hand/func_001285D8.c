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

static inline void MarkRef(GC *pgc, CRef *pref)
{
    switch (pref->m_tagk) {
    case 0x1F: {
        NODE *pn = (NODE *)pref->pobj;
        if (pn->fMark != D_002DD598[0]) {
            pn->fMark = D_002DD598[0];
            PushNode(pgc, pn);
        }
        break;
    }
    case 0x20: {
        CLO *pc = (CLO *)pref->pobj;
        if (pc->fMark != D_002DD598[0]) {
            pc->fMark = D_002DD598[0];
            PushClo(pgc, pc);
        }
        break;
    }
    case 0x22:
        pref->pobj->fMark = D_002DD598[0];
        break;
    }
}

/** @sly2-annot addr=0x1285D8
 * Purpose: Marks one root CRef (pair/proc/method) and pushes it on the GC stacks
 * Info: proposed name CGc_MarkRootRef (T2); class CGc; TU gc
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#gc-0x128138-0x128b60
 * @end-sly2-annot */
void func_001285D8(GC *pgc, CRef *pref)
{
    MarkRef(pgc, pref);
}
