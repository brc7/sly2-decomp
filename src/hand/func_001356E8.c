typedef int ti128 __attribute__((mode(TI)));
struct C { int a[4]; };
struct LSG { ti128 pos; int pad[0x14]; };
struct S { char pad[0xec]; int fec; char pad2[0x16f0 - 0xf0]; ti128 v16f0; };
extern ti128 D_002BC970;
extern ti128 D_002BC980;
extern char D_002DDF40[];
extern char D_003DD490[];
ti128 func_0014F268(void *, ti128);
struct C *func_001AE478(struct C *);
void func_001AE4A0(struct C *, int);
void func_0020FA10();
void IntersectSwBoundingBox(void *, ti128, ti128, void (*)(), int, int *, int *);
int func_0014CC58(int, ti128, ti128, int, int, struct LSG *);
void func_001356E8(struct S *p) {
    struct C ctx;
    struct LSG lsg;
    int cso;
    int aso;
    ti128 v1 = func_0014F268(D_002DDF40, D_002BC970);
    ti128 v2 = func_0014F268(D_002DDF40, D_002BC980);
    func_001AE478(&ctx);
    IntersectSwBoundingBox(D_003DD490, v1, v2, func_0020FA10, p->fec, &cso, &aso);
    if (func_0014CC58(1, v1, v2, cso, aso, &lsg)) p->v16f0 = lsg.pos;
    else p->v16f0 = v2;
    func_001AE4A0(&ctx, 2);
}
