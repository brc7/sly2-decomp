typedef struct CRef { int m_tagk; int m_tag; } CRef;
typedef struct LO { char pad[0xC]; struct LO *pc; char pad2[0x2C]; void *pv; } LO;
typedef struct G { char pad[0x3F0C]; void *pv; } G;
extern G D_003DD490;
void *PloFindSwObject_15E958(G *pg, int k, int a, LO *b, void *c);
void CRef_SetBasic(CRef *pref, void *p);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
void func_00129EC8(CRef *pref);
/** @sly2-annot addr=0x1244E8
 * Purpose: Splice builtin "find-nearest-object": like find-object with k=4
 * Info: proposed name RefOpFindNearestObject (T1); TU bif
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpFindNearestObject__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    LO *b;
    LO *plo;
    void *pv;
    ref.m_tagk = -1;
    b = (LO *)aref[1].m_tag;
    if (aref[2].m_tagk) {
        plo = ((LO *)aref[2].m_tag)->pc;
    } else {
        plo = b;
    }
    {
        G *pg = &D_003DD490;
        int a = aref[0].m_tag;
        CRef_SetBasic(&ref, PloFindSwObject_15E958(pg, 4, a, b, plo ? plo->pv : pg->pv));
    }
    func_001291A8(ret, &ref);
    func_00129EC8(&ref);
    return ret;
}
