typedef int qword __attribute__((mode(TI)));
struct E { char pad[0x18]; float *ag; char pad2[0x28 - 0x1C]; };
struct F { char pad[0x54]; qword *av; char pad2[0x60 - 0x58]; };
struct DL { void *a, *b; int c; };
struct DZG { int x0; int c; struct E *ae; char pad[0x30 - 0xC]; struct DL dl; char pad2[0x60 - 0x3C]; struct F *af; };
struct Y { char pad[0xA8]; int i; };
struct X { char pad[0x40]; qword v; char pad2[0x50 - 0x50]; struct Y *py; };
extern void AppendDlEntry__FP2DLPv(struct DL *pdl, void *pv);
static __inline__ int Dot(qword a, qword b)
{
    qword r, one;
    int g;
    __asm__("vmul.xyz %0, %0, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(r), "=&j"(one) : "0"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(r));
    return g;
}
void AppendDzgDz(struct DZG *pdzg, void *pdz, struct X *px) __asm__("AppendDzgDz__FP3DZGP2DZ");
void AppendDzgDz(struct DZG *pdzg, void *pdz, struct X *px)
{
    int i;
    int k = px->py->i;
    for (i = 0; i < pdzg->c; i++) {
        struct E *pe = &pdzg->ae[i];
        int *ag = (int *)pe->ag;
        if (ag) {
            __asm__ __volatile__("" ::: "memory");
            {
                struct F *pf = &pdzg->af[i];
                ag[pdzg->c] = Dot(px->v, pf->av[k]);
            }
        }
    }
    AppendDlEntry__FP2DLPv(&pdzg->dl, pdz);
    pdzg->c++;
}
