typedef int qword __attribute__((mode(TI)));
struct Q { qword pos; };
struct S { char pad[0x188]; int x188; int x18c; void *x190; };
struct CTX { struct S *p; int x4; void *pv; int xc; int x10; int pad[3]; qword q; char rest[0x30]; };
extern qword D_002A2DD0;
qword func_001A62E8(void *p);
struct Q *func_00182810(struct S *p, qword q);
void func_002333B4(void *pv, int b, int cb);
int func_001F7598(struct CTX *pctx);
void ExplodeExplParams(void *p, int a, int b, qword q1, qword q2, float g, float h);
void func_00181EB0(struct S *p, struct Q *q, int n);
void func_001829D8(struct S *p, void *pv)
{
    struct Q *q;
    struct CTX ctx;
    if (!pv || !p->x188) return;
    q = func_00182810(p, func_001A62E8(pv));
    func_002333B4(&ctx, 0, 0x60);
    ctx.pv = pv;
    ctx.p = p;
    ctx.q = q->pos;
    ctx.xc = 0x100;
    ctx.x10 = -p->x188;
    if (func_001F7598(&ctx)) {
        if (p->x190) ExplodeExplParams(p->x190, 4, 0, D_002A2DD0, q->pos, 0.0f, 0.0f);
        func_00181EB0(p, q, 2);
    }
    p->x18c = 0;
}
