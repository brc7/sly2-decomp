typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
static __inline__ float Dot(qword a, qword b)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
typedef struct { char b[12]; } LEAF;
typedef union { qword q; struct { char pad[0xC]; unsigned short a[2]; } s; } NODE;
struct T { char pad[0xC]; LEAF *aleaf; int cnode; NODE *anode; };
LEAF *PcgtPointInCbspQuick(struct T *p, qword pos, int fl)
{
    union { qword q; float f; } u;
    NODE *pn, *pnEnd, *anode;
    int mask;
    u.q = Make(GetX(pos), GetY(pos), -1.0f);
    pn = p->anode;
    pnEnd = &p->anode[p->cnode];
    anode = p->anode;
    mask = (fl & 1) ? 0x8000 : 0x4000;
    for (; pn != pnEnd; ) {
        unsigned int k;
        k = pn->s.a[(0.0f < Dot(pn->q, u.q)) ? 1 : 0];
        if (k & 0x8000) {
            if (!(k & mask))
                return 0;
            k &= 0x3FFF;
            return &p->aleaf[k];
        }
        pn = anode + k;
    }
    return 0;
}
