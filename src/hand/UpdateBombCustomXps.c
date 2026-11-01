typedef int qword __attribute__((mode(TI)));
struct X { char pad[0x30]; float g30; };
struct LO { char pad[0x28]; int x28; signed char x2C; char x2D; long k:4; long :8; unsigned int f:1; char pad2[0xEC - 0x30]; struct X *px; };
struct N { char pad[0x60]; struct LO *plo; };
struct SO { char pad[0x10]; struct SO *pparent; char pad1[0xE0 - 0x14]; struct SO *pnext; int xE4; int grfE8; struct N *pn; int xF0; };
struct E { qword v0; qword v10; int n20; struct LO *plo; int pad[2]; };
struct C { char pad[0x50]; qword q50; };
struct P { char pad[0xE4]; struct C *pc; int xE8; struct X *px; char pad1[0x1F8 - 0xF0]; int c; struct E *ae; };
struct HIT { qword pos; qword a; qword normal; qword b; qword c; qword d; };
struct G { char pad[0x3F24]; struct SO *psoFirst; };
extern struct G D_003DD490;
extern struct { struct SO *p; } D_003E13B4;
void UpdateSwPox(struct G *pg, struct X *pa, struct X *pb, int n);
qword func_001A62E8(void *p);
int func_0014C748(qword a, qword b, qword c, int n, struct HIT *phit, float g);
void UpdateBombCustomXps(struct P *p)
{
    int i;
    struct SO *pso;
    int unused[4];
    struct HIT hit;
    for (i = 0; i < p->c; i++)
        UpdateSwPox(&D_003DD490, p->px, p->ae[i].plo->px, 2);
    p->c = 0;
    for (pso = D_003E13B4.p; pso; pso = pso->pnext) {
        struct LO *plo;
        struct SO *pp;
        if (!pso->xF0)
            continue;
        if (!(pso->grfE8 & 0x100))
            continue;
        plo = 0;
        for (pp = pso->pparent; pp; pp = pp->pparent) {
            if (pp->pn) {
                plo = pp->pn->plo;
                break;
            }
        }
        if (!plo)
            continue;
        if (!plo->f)
            continue;
        {
            qword a = func_001A62E8(pso);
            struct X *px = plo->px;
            qword b = p->pc->q50;
            if (func_0014C748(a, b, func_001A62E8(p), 1, &hit, px->g30)) {
                struct E *pe;
                qword d, t;
                register qword vp __asm__("vf2");
                register qword vn __asm__("vf1");
                register int n __asm__("$2");
                UpdateSwPox(&D_003DD490, p->px, plo->px, 1);
                pe = &p->ae[p->c++];
                pe->plo = plo;
                __asm__("" : "+r"(pe));
                vp = hit.pos;
                pe->v0 = vp;
                vn = hit.normal;
                __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(t) : "j"(vp), "j"(vn));
                __asm__("qmfc2.ni %0, %1" : "=r"(n) : "j"(d));
                pe->v10 = vn;
                pe->n20 = n;
                if (p->c >= 4)
                    break;
            }
        }
    }
}
