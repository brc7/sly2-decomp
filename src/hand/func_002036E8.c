struct G;
extern struct G D_003DD490;
struct S { char pad[0xEC]; void *o; int n; };
void RequestTaskState(void *o, int n, void *p);
char *PpoHero(struct G *pg);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *o, int n);
void func_0019D948(void *pv);
void func_002036E8(struct S *p, int n)
{
    if (p->o) {
        char *o;
        RequestTaskState(p->o, 2, p);
        o = PpoHero(&D_003DD490);
        if (o && FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x34)) func_0019D948(o + 0xF24);
    } else p->n = n;
}
