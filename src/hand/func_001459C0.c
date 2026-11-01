typedef int qword __attribute__((mode(TI)));
struct E;
struct D { char pad[0x10]; struct E *pe; };
struct C { char pad[0xDC]; struct D *pd; };
struct A { char pad[0xE8]; struct C *pc; };
struct SO { char pad[0x18]; int k; };
struct X30 { char pad[0x2DC]; int x2DC; };
struct P { char pad[0x10]; struct A *pa; char pad2[0x30 - 0x14]; struct X30 *p30; char pad3[0x80 - 0x34]; qword pos; };
struct SO *func_00145AA8(struct P *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
qword func_001A62E8(void *p);
int func_001C09A0(struct SO *pso, qword pos, int n);
int func_00173610(qword *ppos, float a, float b, int c, int d);
int func_001459C0(struct P *p)
{
    struct SO *pso = func_00145AA8(p);
    struct D *pd;
    struct E *pe;
    qword pos;
    if (!pso) return 0;
    pd = p->pa->pc ? p->pa->pc->pd : 0;
    pe = pd ? pd->pe : 0;
    if ((struct E *)pso == pe) return 0;
    if (pso->k == 0x2CA) return 0;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->p30, 0x34) && p->p30->x2DC != -1) return 0;
    pos = func_001A62E8(p->p30);
    if (!func_001C09A0(pso, pos, 0)) return 0;
    if (!func_00173610(&pos, 100.0f, -150.0f, 0, 0)) return 0;
    p->pos = pos;
    return 1;
}
