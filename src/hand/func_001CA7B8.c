struct RC { int cRef; };
struct E { int x0; int x4; struct RC *x8; int pad[3]; };
struct S { char pad[0xEC]; int c; struct E *ae; };
struct AL { int cb; int cbAlign; };
struct SP { struct RC *p; };
void func_002311B0(struct S *p, struct S *q);
struct E *func_001AE238(struct AL *pal, struct E *pv);
void CFrame_CloneTo(struct SP *pret, struct RC *p, struct S *ps);
void func_00257078(struct RC *p, int n);
void func_00259A88(struct RC *p);
void func_001CA7B8(struct S *p, struct S *q)
{
    int i;
    union { struct AL al; struct SP sp; } u;
    func_002311B0(p, q);
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        struct RC *prc;
        if (!pe->x8) continue;
        if (p->ae == q->ae) {
            u.al.cb = p->c * sizeof(struct E);
            u.al.cbAlign = 0x10;
            p->ae = func_001AE238(&u.al, p->ae);
            pe = &p->ae[i];
        }
        CFrame_CloneTo(&u.sp, pe->x8, p);
        pe->x8 = u.sp.p;
        prc = u.sp.p;
        if (prc && --prc->cRef == 0) func_00257078(prc, 3);
        func_00259A88(pe->x8);
    }
}
