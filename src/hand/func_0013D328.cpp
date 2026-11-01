extern "C" { void *func_001291A8(void *, const void *); void func_00129EC8(void *); }
class CRef {
public:
    int m_tagk;
    int m_tag;
    CRef(int tagk, int n) { m_tagk = tagk; m_tag = n; }
    CRef(const CRef &ref) { func_001291A8(this, &ref); }
    ~CRef() { func_00129EC8(this); }
};
struct FRAME {
    int a[4];
    FRAME();
    ~FRAME();
};
struct S { char pad[0x194]; void *f194; };
struct O;
extern char D_003D4A60[];
extern "C" {
void SetBootyState(void *, int);
O *func_0013D1A8(S *);
void func_0013CF38(S *, int);
CRef func_00258200(O *, const CRef &, void *);
}
static inline void SetPv(O *pv, const CRef &r, void *) { CRef arg(r); const CRef &ret = func_00258200(pv, arg, D_003D4A60); }
extern "C" void func_0013D328(S *p, int n)
{
    if (p->f194) SetBootyState(p->f194, 0);
    FRAME frame;
    {
        O *po = func_0013D1A8(p);
        CRef ref(0x15, n);
        SetPv(po, ref, D_003D4A60);
    }
    if (n != -1) func_0013CF38(p, 0);
}
