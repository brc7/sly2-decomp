struct CRef { int m_tagk; void *pv; };
struct N;
struct SP { struct N *p; };
struct N { int cref; int x4; struct CRef ref; struct SP sp; };
struct X { int f; struct N *pn; };
extern char D_004520A8[];
struct N *PvNewPair(void);
void func_00256C38(struct SP *psp, struct N *pn);
void PairPtr_Assign(struct SP *psp, struct N *pn);
void PairPtr_Release(struct SP *psp, int n);
struct CRef *CRef_SetPair(struct CRef *pref, struct N *pn);
void CRef_SetSymid(struct CRef *pref, int n);
void CRef_SetBasic(struct CRef *pref, int n);
void Decref__4CRef(struct CRef *pref);
void GcPair_Unlink(void *pv);
void func_0011CD98(void *pool, void *pv);
/** @sly2-annot addr=0x132ED8
 * Purpose: Build the Splice call list (symid, self as BASIC, then the arg pair list) for an event
 * Info: proposed name PpairBuildSpliceEventArgs (T2); TU basic
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
struct SP *func_00132ED8(struct SP *ret, int a1, int a2, struct X *px)
{
    struct SP sp;
    struct N *p;
    func_00256C38(&sp, PvNewPair());
    {
        struct CRef *pref = &sp.p->ref;
        CRef_SetPair(pref, PvNewPair());
    }
    p = (struct N *)sp.p->ref.pv;
    CRef_SetSymid(&p->ref, a2);
    {
        struct N *pn = PvNewPair();
        PairPtr_Assign(&p->sp, pn);
    }
    p = p->sp.p;
    CRef_SetBasic(&p->ref, a1);
    if (px && px->f)
        PairPtr_Assign(&p->sp, px->pn);
    func_00256C38(ret, sp.p);
    {
        struct N *pt = sp.p;
        if (pt && --pt->cref == 0) {
            PairPtr_Release(&pt->sp, 2);
            Decref__4CRef(&pt->ref);
            GcPair_Unlink(pt);
            func_0011CD98(D_004520A8, pt);
        }
    }
    return ret;
}
