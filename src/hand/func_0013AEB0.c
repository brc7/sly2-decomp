typedef int qword __attribute__((mode(TI)));
typedef struct VTE { short d; short pad; qword (*pfn)(void *); } VTE;
typedef struct VT { VTE a[64]; } VT;
typedef struct E { struct P *p; void *x; int pad[14]; } E;
typedef struct Q { int x0, x4; E ae[2]; char pad88[0x24]; struct Q *pqNext; } Q;
#define AE(q) ((q)->ae)
static inline int IOther(Q *pq, struct P *p)
{
    int i = p != AE(pq)[0].p;
    i = 1 - i;
    return i;
}
typedef struct L { Q *pq; struct L *next; } L;
typedef struct O { char pad[0x50]; qword pos; } O;
typedef struct W { char pad[0x64]; L *pl; } W;
typedef struct P {
    int x0, x4, x8;
    VT *pvt;
    char pad10[0xE4 - 0x10];
    O *po;
    int xE8;
    W *pw;
    char padF0[0x150 - 0xF0];
    int k;
    char pad154[0x174 - 0x154];
    int x174;
    char pad178[0x1E4 - 0x178];
    int x1E4;
    int x1E8;
} P;
typedef struct K { short x; short y; int i; } K;

extern float D_002A4B18;
extern qword D_002A2DA0;
extern K D_002A4B10;
extern char D_003DD490[];
void func_001F4F10(P *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(P *p, int n);
int func_001C4098(P *p);
int func_0013B1C8(P *p, void *x);
void func_0013B428(P *p, qword pos, qword dir);
void func_0013B5C8(P *p, float g);
void PostSwCallback(void *pg, P *p, K k, void *x);

static inline qword Length(qword a)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\t"
            "vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q"
        : "=j"(r), "=j"(one) : "j"(a));
    return r;
}
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(c));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    return l;
}
static inline qword Sub(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword DivQ(qword v, qword f)
{
    qword r;
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(v), "j"(f));
    return r;
}
static inline float GetX(qword q)
{
    float f;
    __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q));
    return f;
}

static inline qword NormalizeSafe(qword v, qword def)
{
    qword len = Length(v);
    if (GetX(len) < 0.0001f)
        return def;
    return DivQ(v, len);
}
void func_0013AEB0(P *p)
{
    qword pos;
    float gBest;
    Q *pqBest;
    L *pl;
    Q *pq;

    func_001F4F10(p);
    switch (p->k) {
    case 0:
    case 2:
        if (p->x174)
            return;
        break;
    case 1:
        if (!p->x1E4)
            return;
        if (p->x1E8)
            return;
        break;
    }
    pos = p->po->pos;
    gBest = D_002A4B18;
    pqBest = 0;
    for (pl = p->pw->pl; pl; pl = pl->next) {
        for (pq = pl->pq; pq; pq = pq->pqNext) {
            float g = GetX(Length(Sub(*(qword *)AE(pq)[0].pad, pos)));
            int f;
            if (gBest <= g)
                continue;
            f = 0;
            switch (p->k) {
            case 0:
                f = 1;
                break;
            case 2: {
                P *pOther = AE(pq)[IOther(pq, p)].p;
                if (FIsBasicDerivedFrom__FP5BASIC3CID(pOther, 0x32)) {
                    f = 0;
                    if (func_001C4098(pOther))
                        f = 1;
                }
                break;
            }
            case 1:
                if (func_0013B1C8(p, AE(pq)[IOther(pq, p)].x))
                    f = 1;
                break;
            }
            if (f) {
                gBest = g;
                pqBest = pq;
            }
        }
    }
    if (!pqBest)
        return;
    switch (p->k) {
    case 0:
    case 2: {
        qword posT = *(qword *)AE(pqBest)[0].pad;
        VTE *pe = &p->pvt->a[33];
        qword v = pe->pfn((char *)p + pe->d);
        register qword n __asm__("vf1");
        register qword kDef __asm__("$6");
        qword l;
        float gl;
        __asm__("" : "=r"(kDef) : "0"(D_002A2DA0));
        l = VLen(v);
        __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
        if (gl < 1e-4f) {
            n = kDef;
        } else {
            qword rn;
            __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(rn) : "j"(v), "j"(l));
            n = rn;
        }
        func_0013B428(p, posT, n);
        func_0013B5C8(p, 0.0f);
        break;
    }
    case 1:
        PostSwCallback(D_003DD490, p, D_002A4B10, AE(pqBest)[IOther(pqBest, p)].x);
        break;
    }
}
