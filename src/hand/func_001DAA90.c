typedef struct { char pad[0x1C]; int fl; } R;
typedef struct {
    char pad[0x10];
    void *x10;
    char pad14[0x24 - 0x14];
    void *x24;
    char pad28[0x30 - 0x28];
    int x30;
    char pad34[0x40 - 0x34];
    int x40;
    char pad44[4];
    int x48;
} OBJ;
extern struct G D_003DD490;
extern int D_002C3258[];
void func_001DA808(OBJ *);
int func_00187D80(void *, int);
void *PpoHero(struct G *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void func_001DB520(OBJ *, int);
void func_001DA858(OBJ *);
R *func_001DAE20(OBJ *, int);
void func_001DA950(OBJ *, int, int);
void *func_001FBB90(int);
void func_001FB2F8(void *, int);
void func_00141FC8(void *, int, int);
void func_001DAA90(OBJ *p)
{
    void *pv;
    R *r;
    int i;
    func_001DA808(p);
    if (p->x24 == 0)
        return;
    if (p->x40 == -1)
        return;
    if (p->x10 == 0)
        return;
    if (p->x30 && func_00187D80(p->x10, 0x10)) {
        func_001DB520(p, -1);
        return;
    }
    pv = PpoHero(&D_003DD490);
    if (pv == 0 || !FIsBasicDerivedFrom__FP5BASIC3CID(pv, 0x34)) {
        func_001DB520(p, -1);
        return;
    }
    func_001DA858(p);
    if (func_00187D80(p->x10, 0x7E8)) {
        r = func_001DAE20(p, p->x48);
        if (r == 0 || !(r->fl & 1))
            func_001DA950(p, 1, 0xE2);
        else
            func_001DA950(p, 1, 0x31);
    }
    for (i = 0; i < 3; i++) {
        if (!func_00187D80(p->x10, D_002C3258[i]))
            continue;
        r = func_001DAE20(p, p->x48);
        if (r == 0 || !(r->fl & 4)) {
            func_001DA950(p, 1, 0x32);
        } else if (!(r->fl & 1)) {
            func_001DA950(p, 1, 0x33);
        } else {
            func_001FB2F8(func_001FBB90(0x34), 0);
            func_00141FC8(pv, i, p->x48);
        }
    }
}
