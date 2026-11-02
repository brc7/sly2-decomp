typedef int qword __attribute__((mode(TI)));
struct SLOTHEAP { int a[6]; };
extern SLOTHEAP D_00452170, D_00452188, D_004521A0, D_004521B8, D_004521D0, D_004521E8;
extern char D_004520A8[];
extern "C" {
void func_0011CD98(void *psh, void *pv);
void GcPair_Unlink(void *pv);
void GcPool6_Unlink(void *pv);
}
struct NODE;
struct SPM { NODE *p; ~SPM(); };
struct VT { short d; short pad; void (*pfn)(void *); };
struct O21 { char pad[0xC]; VT *vt; };
struct CRef {
    int m_tagk;
    void *m_pv;
    void Decref();
    ~CRef() { Decref(); }
};
struct NV { qword v; int cref; };
struct NM { qword m[4]; int cref; };
struct N2 { int a[2]; int cref; };
struct N3 { int a[3]; int cref; };
struct NODE {
    int cref;
    int x4;
    CRef ref;
    SPM pnext;
    void operator delete(void *pv) { GcPair_Unlink(pv); func_0011CD98(D_004520A8, pv); }
};
struct O20 { int cref; ~O20(); };
struct O22 {
    int cref;
    ~O22() { GcPool6_Unlink(this); }
    void operator delete(void *pv) { func_0011CD98(&D_00452170, pv); }
};
/** @sly2-annot addr=0x129EC8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4,
 *   docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
void CRef::Decref()
{
    switch (m_tagk) {
    case 0x1A: {
        NV *p = (NV *)m_pv;
        if (--p->cref == 0)
            func_0011CD98(&D_00452188, p);
        break;
    }
    case 0x1B: {
        NM *p = (NM *)m_pv;
        if (--p->cref == 0)
            func_0011CD98(&D_004521A0, p);
        break;
    }
    case 0x1C: {
        NV *p = (NV *)m_pv;
        if (--p->cref == 0)
            func_0011CD98(&D_004521B8, p);
        break;
    }
    case 0x1D: {
        N2 *p = (N2 *)m_pv;
        if (--p->cref == 0)
            func_0011CD98(&D_004521D0, p);
        break;
    }
    case 0x1E: {
        N3 *p = (N3 *)m_pv;
        if (--p->cref == 0)
            func_0011CD98(&D_004521E8, p);
        break;
    }
    case 0x1F: {
        NODE *p = (NODE *)m_pv;
        if (--p->cref == 0)
            delete p;
        break;
    }
    case 0x20: {
        O20 *p = (O20 *)m_pv;
        if (--p->cref == 0)
            delete p;
        break;
    }
    case 0x21: {
        O21 *p = (O21 *)m_pv;
        if (p)
            p->vt[2].pfn((char *)p + p->vt[2].d);
        break;
    }
    case 0x22: {
        O22 *p = (O22 *)m_pv;
        if (--p->cref == 0 && p) {
            GcPool6_Unlink(p);
            func_0011CD98(&D_00452170, p);
        }
        break;
    }
    }
}
