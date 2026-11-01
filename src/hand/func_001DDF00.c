struct O { char pad[0x5C]; int n; };
struct E { struct O *po; int a, b; };
struct G2 { char pad[0x48]; int c; struct E ae[1]; };
extern struct G2 D_003DA3C0;
extern struct G D_003DD490;
void *PchzFindSwLangstr__FP2SWi(struct G *g, int n);
void func_0021ED58(void *p, void *q);
void func_001DDF00(void *p, int i) {
    struct G2 *pg = &D_003DA3C0;
    if (i < pg->c) func_0021ED58(p, PchzFindSwLangstr__FP2SWi(&D_003DD490, pg->ae[i].po->n));
}
