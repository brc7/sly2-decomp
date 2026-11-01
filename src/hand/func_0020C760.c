struct G;
extern struct G D_003DD490;
struct O { char pad[0x58]; int n58; char p2[0xEC]; unsigned long fl; };
struct S { char pad[0x34]; struct O *o; char p2[0x28]; int f60; int p64; char x68[4]; };
void ReleaseAct(void *pv);
void func_002303A0(struct O *o, int n, int m);
void RecalcSwOxfFilterForObject(struct G *pg, struct O *o);
void func_0020C760(struct S *p)
{
    if (p->f60) {
        p->f60 = 0;
        if (*(int *)p->x68) {
            ReleaseAct(p->x68);
            func_002303A0(p->o, p->o->n58, 2);
            p->o->fl &= ~(1L << 26);
            RecalcSwOxfFilterForObject(&D_003DD490, p->o);
        }
    }
}
