typedef unsigned long ulong;
struct Q { char pad[0x158]; int n158; };
struct Ent { int a0; int a4; int a8; int aC; };
struct P { char pad[0x2C0]; ulong l2C0; char pad2[0x404 - 0x2C8]; struct Ent a[8]; int n484; char pad3[0x494 - 0x488]; struct Q *pq; };
struct G;
extern struct G D_003DD490;
extern int D_002E1E40;
ulong func_00207A20(int);
int PpoHero(struct G *);
void func_001F17F8(struct P *, int);
void func_001F1900(struct P *p)
{
    ulong m;
    int k;
    if (!p->pq) return;
    k = -1;
    if (p->pq->n158) {
        m = func_00207A20(0x1D) | func_00207A20(0x20) | func_00207A20(5) | func_00207A20(6)
            | func_00207A20(7) | func_00207A20(0x17) | func_00207A20(0x16);
        if (p->l2C0 & m) k = 0;
        else if (!p->n484 || p->a[p->n484 - 1].a0 == 0x2B3 || !PpoHero(&D_003DD490) || PpoHero(&D_003DD490) != D_002E1E40) k = 1;
        else k = 2;
    }
    func_001F17F8(p, k);
}
