/* MATCH (Claude agent, lane D4): v2 inline Dot(qword *pa, qword *pb) (keeps &ax[i].normal hoisted in a reg), returning int from a qmfc2 into `register int d asm("$7")`, stored through `*(int *)pg++` (sw $a3 like the target). */
typedef int qword __attribute__((mode(TI)));
struct X { qword pos; qword normal; qword x20; };
struct E { int a, b, c; };
struct LO { char pad[0x70]; int c70; struct E *ae74; };
struct AL { int cb; int cbAlign; };
extern float D_002BD8D4;
void func_001AE478(void *p);
void func_001AE4A0(void *p, int n);
void *func_001AE318(struct AL *pal);
int func_001A93C8(qword pos, struct LO *po, struct E *pe, float g, int a, int b, int cMax, struct X *ax);
void SolveInequalities(int c, float *aDot, float *aW, float *aZ, float g);
qword LimitVectorLength(qword v, float g);

static inline int Dot(qword *pa, qword *pb)
{
    register qword nj __asm__("vf1") = *pb;
    register qword t __asm__("vf2") = *pa;
    qword one;
    register int d __asm__("$7");
    __asm__("vmul.xyz %0, %0, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "+j"(t), "=&j"(one) : "j"(nj));
    __asm__("qmfc2.ni %0, %1" : "=r"(d) : "j"(t));
    return d;
}

static inline void *PvAlloc(int cb, int cbAlign)
{
    struct AL al;
    al.cb = cb;
    al.cbAlign = cbAlign;
    return func_001AE318(&al);
}

qword func_0014EDA0(void *p, int c, struct LO **apo, float (*pfn)(void *, qword, qword, struct X *),
                    qword pos, qword v, float g, float h)
{
    struct X ax[64];
    int n = 0;
    int i, j;

    {
    int k;
    for (k = 0; k < c; k++) {
        struct LO *po = apo[k];
        n += func_001A93C8(pos, po, &po->ae74[po->c70], g, 0, 0, 64 - n, &ax[n]);
    }
    }
    if (n == 0)
        return v;
    {
        struct { int a[4]; } frame;
        float *pg;
        qword r, sum, z;
        float *aDot, *aW, *aZ;
        func_001AE478(&frame);
        __asm__("vsub.xyzw %0, $vf0, $vf0" : "=j"(z));
        aDot = PvAlloc(n * n * 4, 0x10);
        aW = PvAlloc(n * 4, 0x10);
        aZ = PvAlloc(n * 4, 0x10);
        pg = aDot;
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++)
                *(int *)pg++ = Dot(&ax[i].normal, &ax[j].normal);
        }
        pg = aW;
        for (i = 0; i < n; i++)
            *pg++ = pfn(p, pos, v, &ax[i]);
        SolveInequalities(n, aDot, aW, aZ, D_002BD8D4 * h);
        pg = aZ;
        sum = z;
        for (i = 0; i < n; i++) {
            union { qword q; float f[4]; } s;
            __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(*pg++));
            __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(sum) : "0"(sum), "j"(ax[i].normal), "j"(s.q));
        }
        {
            qword q;
            __asm__("qmfc2.ni %0, %1" : "=r"(q) : "j"(sum));
            r = LimitVectorLength(q, h);
        }
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(v), "j"(r));
        func_001AE4A0(&frame, 2);
        return r;
    }
}
