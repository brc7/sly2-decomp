struct T { char pad[0xCC]; int a[1]; };
extern struct G D_003DD490;
int PchzFindSwLangstr__FP2SWi(struct G *g, int x);
void func_0021ED58(void *p, int v);
void func_00211348(void *p, int i, struct T *t)
{
    func_0021ED58(p, PchzFindSwLangstr__FP2SWi(&D_003DD490, t->a[i]));
}
