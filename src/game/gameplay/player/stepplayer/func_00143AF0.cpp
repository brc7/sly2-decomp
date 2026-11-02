struct CRef;
extern "C" void func_00129EC8(CRef *);
extern "C" void func_001291A8(CRef *, const CRef *);
struct CRef {
    int tagk; int tag;
    CRef() { tagk = -1; }
    CRef(int n, int k) { tag = n; tagk = k; }
    CRef(int k, int n, int) { tagk = k; tag = n; }
    CRef(const CRef &r) { func_001291A8(this, &r); }
    ~CRef() { func_00129EC8(this); }
};
struct CRefRaw { int tagk; int tag; };  /* same layout, no ctor/dtor */
struct VTE { short d; short pad; void *(*pfn)(void *); };
struct LO { char pad[0xC]; VTE *vt; char pad2[0x18 - 0x10]; void *p18; };
struct G;
extern "C" G D_003D4A60;
extern char D_002A4DB0[];
extern "C" {
void *PkeyFindBootySaveVar(void *p);
CRef RefSetGlobalSaveVar(void *p, const CRef &ref, G *pv);
CRef RefSetSaveVarV(const CRef &ref, void *o, void *pv, int n);
}
/* Same SetPv form as func_0013D328: the unused dummy param makes gcc set up %hi(D_003D4A60)
   before the copy (lui in the copy-ctor jal slot). The copy is made in a raw struct with explicit
   copy/dtor calls: an inlined ~CRef would put &arg in a pseudo that gcse PRE then shares with
   the second CRef temp at the same stack slot. */
static inline void SetPv(void *pv, const CRef &r, void *)
{
    CRefRaw arg;
    func_001291A8((CRef *)&arg, &r);
    {
        const CRef &ret = RefSetGlobalSaveVar(pv, *(CRef *)&arg, &D_003D4A60);
    }
    func_00129EC8((CRef *)&arg);
}
/** @sly2-annot addr=0x143AF0
 * Purpose: Mark the assembly save slot for booty x and set oidBooty=-1
 * Info: proposed name ClearStepplayerBootySave (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
extern "C" void *func_00143AF0(LO *p, int x)
{
    void *pv = PkeyFindBootySaveVar(p->p18);
    if (pv) {
        CRef a(x, 0x15);
        SetPv(pv, a, &D_003D4A60);
    }
    VTE *e = &p->vt[103];
    CRef ret = RefSetSaveVarV(CRef(0x15, -1, 0), e->pfn((char *)p + e->d), D_002A4DB0, 0);
    return pv;
}
