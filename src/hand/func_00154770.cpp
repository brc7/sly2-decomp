typedef int qword __attribute__((mode(TI)));
struct Z { char pad[0xE4]; int xE4; char pad1[0x194 - 0xE8]; int x194; };
struct SO { char pad[0x4C8]; int x4C8; int x4CC; char pad1[0x4D4 - 0x4D0]; Z *x4D4; char pad2[0x1040 - 0x4D8]; int x1040; };
struct VECTOR { qword q; VECTOR() {} VECTOR(const VECTOR &v) : q(v.q) {} };
extern "C" {
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int k);
qword func_001A62E8(void *p);
VECTOR *func_001A6318(SO *p);
int func_00154770(void *p, SO *pso);
}
static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=&j"(d) : "j"(a), "j"(b)); return d; }
static inline float Dot(qword a, const VECTOR &b)
{
    qword t;
    float g;
    __asm__("vmul.xyz %0, %0, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "+j"(a), "=&j"(t) : "j"(b.q));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
    return g;
}

int func_00154770(void *p, SO *pso)
{
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(pso, 0x34)) return 0;
    if (pso->x4CC == -1) return 0;
    VECTOR d; d.q = Sub(func_001A62E8(pso), func_001A62E8(p));
    switch (pso->x4C8) {
    case 0:
    case 13:
        if (FIsBasicDerivedFrom__FP5BASIC3CID(pso->x4D4, 0xC)) {
            if (pso->x4D4->x194) return 0;
        } else if (FIsBasicDerivedFrom__FP5BASIC3CID(pso->x4D4, 0x22)) {
            if (pso->x4D4->xE4) return 0;
        } else {
            return !pso->x4D4->xE4;
        }
        {
            bool f = Dot(d.q, *func_001A6318(pso)) <= 0.0f;
            bool g = false;
            if (pso->x4C8 == 0) g = !pso->x1040;
            if (f == g) break;
            return 1;
        }
    case 1:
        if (pso->x4CC) break;
        return 1;
    case 2:
        return 1;
    case 8:
        if (pso->x4CC == 9 || pso->x4CC == 10) return 1;
        break;
    }
    return 0;
}
