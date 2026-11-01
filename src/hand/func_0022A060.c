typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; int (*pfn)(); };
struct J { char pad[0xC]; struct VTE *vt; char pad1[0x2DC - 0x10]; int x2DC; };
struct P { char pad[0xC]; struct VTE *vt; char pad1[0x150 - 0x10]; int x150; int x154; float g158; float g15C; int x160; int x164; };
struct G { char pad[0x3B0C]; int x3B0C; };
extern struct G D_003DD490;
void func_001F36E0(struct P *p);
struct J *PpoHero(struct G *pg);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct J *pj, int k);
qword func_001A62E8(void *p);
static __inline__ float GDist(qword a, qword b)
{
    qword d, t;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t));
    __asm__("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
void func_0022A060(struct P *p)
{
    struct J *pj;
    int f;
    float r;
    func_001F36E0(p);
    pj = PpoHero(&D_003DD490);
    if (pj && FIsBasicDerivedFrom__FP5BASIC3CID(pj, p->x154)) {
        r = p->x164 ? p->g15C : p->g158;
        f = GDist(func_001A62E8(pj), func_001A62E8(p)) < r;
    } else f = 0;
    if ((p->x150 == 0 || p->x150 == 2) && D_003DD490.x3B0C)
        f = 0;
    if (f && FIsBasicDerivedFrom__FP5BASIC3CID(pj, 0x34) &&
        (pj->x2DC != -1 || ((int (*)(void *))pj->vt[77].pfn)((char *)pj + pj->vt[77].d) == 2))
        f = p->x164;
    if (f != p->x164) {
        p->x164 = f;
        if (f)
            ((void (*)(void *, int, int, int, int, int))p->vt[3].pfn)((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
        else
            ((void (*)(void *, int, int, int, int, int))p->vt[3].pfn)((char *)p + p->vt[3].d, 6, 0, 0, 0, 0);
    }
}
