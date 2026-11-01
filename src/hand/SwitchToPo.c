struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; char p1[0x18 - 0x10]; int oid; };
struct G { char pad[0x3EFC]; struct LO *plo3EFC; struct LO *plo3F00; };
struct CRef { int m_tagk; int m_tag; int pad[2]; };
struct LO *PpoCur(struct G *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct LO *plo, int n);
struct CRef *func_001D8B20(struct CRef *pret, struct CRef *pref, const char *s, int n);
void func_00129EC8(struct CRef *p);
void func_00194F70(struct G *p, int a, int b, int c);
void func_001C48C0(struct G *p);
#define VCALL(o, n, a) ((void (*)(void *, int))(o)->vt[n].pf)((char *)(o) + (o)->vt[n].d, a)
void SwitchToPo(struct G *p, struct LO *plo)
{
    struct LO *cur = PpoCur(p);
    struct LO *nw = p->plo3F00;
    if (!nw) nw = plo;
    if (cur != nw && cur) VCALL(cur, 78, 0);
    p->plo3EFC = plo;
    if (cur != nw && nw) VCALL(nw, 78, 1);
    if (plo && FIsBasicDerivedFrom__FP5BASIC3CID(plo, 0x34)) {
        struct CRef ret, ref;
        ref.m_tagk = 0x15;
        ref.m_tag = plo->oid;
        func_001D8B20(&ret, &ref, "oidPoCurrent", 0);
        func_00129EC8(&ret);
        func_00129EC8(&ref);
    }
    func_00194F70(p, 0, 0, 0);
    func_001C48C0(p);
}
