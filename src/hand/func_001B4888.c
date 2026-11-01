struct REQ { int cb; int align; };
struct T { int x0; void *x4; void *x8; void *xC; };
struct P { char pad[0x15F0]; void *x15F0; void *x15F4; void *x15F8; void *x15FC; char pad2[0x1610 - 0x1600]; struct T *x1610; struct T *x1614; };
extern char D_002C0270[];
void func_002039C0(struct P *p);
void BindLoOidTable(struct P *p, int n, void *q);
struct T *func_001AE2A0(struct REQ *r);
void *func_001F4C58(struct P *p, int a, int b);
void func_00186930(struct T *t);
static __inline__ struct T *NewT(void) { struct REQ req; req.cb = 0xE0; req.align = 0x10; return func_001AE2A0(&req); }
void func_001B4888(struct P *p)
{
    func_002039C0(p);
    BindLoOidTable(p, 0x89, D_002C0270);
    p->x1610 = NewT();
    p->x1610->x4 = func_001F4C58(p, 0x200, 6);
    p->x1610->x8 = func_001F4C58(p, 0x201, 6);
    p->x1610->xC = func_001F4C58(p, 0x202, 1);
    func_00186930(p->x1610);
    p->x1614 = NewT();
    p->x1614->x4 = func_001F4C58(p, 0x203, 6);
    p->x1614->x8 = func_001F4C58(p, 0x204, 6);
    p->x1614->xC = func_001F4C58(p, 0x205, 1);
    func_00186930(p->x1614);
    p->x15F0 = p->x1610->xC;
    p->x15F4 = func_001F4C58(p, 0x202, 6);
    p->x15F8 = p->x1614->xC;
    p->x15FC = func_001F4C58(p, 0x205, 6);
}
