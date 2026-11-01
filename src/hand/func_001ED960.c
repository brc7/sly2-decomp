struct E { int a; void *pv; int b; void *powner; int dle; };
struct P { char pad[0x24]; int x24; char pad2[0x34 - 0x28]; int dl; };
extern int D_003DA790;
extern struct E D_003DA798[];
void ClearSle(int *pdle);
void PrependSlEntry(int *pdl, struct E *pe);
void func_00226E88(void *a, int b, int c, void *d, struct E *pe, int *pb);
void func_001EC718(struct E *pe);
int func_001ED960(struct P *p, void *a1, void *a2)
{
    struct E e;
    struct E *pe;
    if (D_003DA790 < 0x200) {
        pe = &D_003DA798[D_003DA790++];
        ClearSle(&pe->dle);
        PrependSlEntry(&p->dl, pe);
        pe->powner = p;
    } else {
        pe = &e;
    }
    func_00226E88(a2, 0xD, 1, a1, pe, &pe->b);
    pe->pv = &p->x24;
    func_001EC718(pe);
    return 1;
}
