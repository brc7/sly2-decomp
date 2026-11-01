typedef int qword __attribute__((mode(TI)));
struct VE { short d; short pad; unsigned int (*pf)(void *); };
struct EC { char pad[0x60]; void *p60; };
struct X4 { char pad[0xEC]; struct EC *pec; };
struct X { int pad; struct X4 *p4; };
struct S { char pad[0xC]; struct VE *vt; char pad2[0x218 - 0x10]; qword *p218; qword *p21C; char pad3[0x258 - 0x220]; void *p258; };
struct HIT { qword pos; qword q10; qword normal; qword q30; float t; int n44; int n48; int n4C; char pad[0x10]; };
extern qword D_002A2DC0;
extern char D_003DD490[];
void func_001F5990(void);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
qword func_001A62E8(void *p);
int func_0014C6D8(struct X *x, int a, qword pos, qword v, int n, struct HIT *phit);
int func_002048C8(struct S *p, qword v, struct X *x, int n);
void func_001A7C18(void *g, void *p, struct X *x, qword a, qword b, int k, int c, int d, int e, float f);

void func_00204AC8(struct S *p, struct X *x, int a2, int a3, int a4, int a5)
{
    unsigned int fl;
    struct HIT hit;
    qword pos, n, d, tmp;
    float f;
    func_001F5990();
    fl = p->vt[85].pf((char *)p + p->vt[85].d);
    if (!(fl & 1))
        return;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(x->p4->pec->p60, 0x33))
        return;
    pos = func_001A62E8(p);
    if (func_0014C6D8(x, a4, pos, *((fl & 2) ? p->p21C : p->p218), 1, &hit) <= 0)
        return;
    if (0.0f < hit.t) {
        if (fl & 8)
            n = hit.normal;
        else if (0.7f < ((float *)&hit.normal)[2])
            n = D_002A2DC0;
        else if (hit.n48)
            n = hit.normal;
        else if (func_002048C8(p, *p->p218, x, hit.n4C))
            n = D_002A2DC0;
        else
            n = hit.normal;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(*p->p218), "j"(hit.pos));
        __asm__("vmul.xyz %0, %0, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "+j"(d), "=&j"(tmp) : "j"(n));
        __asm__("mtc1 %1, %0" : "=f"(f) : "r"(d));
        func_001A7C18(D_003DD490, p->p258, x, *p->p218, n, 0xE, 0, 0, a5, f);
    }
}
