typedef int qword __attribute__((mode(TI)));
struct V2 { float x, y; };
struct S { char pad[0x40]; int f; char pad2[0xF8]; int a; char pad3[0x24]; int b; char pad4[8]; float c, d, e, g; int h; char pad5[0x1C]; qword q; struct V2 v; };
extern qword D_002C41C0;
extern struct V2 D_002C41D0;
void InitXfm(struct S *p);
void SetLoBusyk(struct S *p, int a);
static inline int FIsPtr(int f) { return !(f & 1); }
void func_00220ED0(struct S *p)
{
    int f;
    InitXfm(p);
    p->e = 2.0f; /*P*/
    p->g = 1.0f; /*P*/
    p->c = 2.0f; /*P*/
    p->d = 1.0f; /*P*/
    p->h = -1; /*P*/
    p->q = D_002C41C0; /*P*/
    p->v = D_002C41D0; /*P*/
    p->a = -1; /*P*/
    p->b = 7; /*P*/
    SetLoBusyk(p, 1);
    f = p->f;
    if (FIsPtr(f)) *(int *)f |= 2;
    else p->f = f | 2;
}
