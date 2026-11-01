struct E { int n; int a; };
struct G2 { char pad[0x4C]; struct E ae[0x40]; int nPad; int i; };
extern struct G2 D_003DA150;
struct T { int a[5]; int id; int b[2]; };
extern struct T D_002BCD00[];
extern struct G D_003DD490;
void *PchzFindSwLangstr__FP2SWi(struct G *g, int n);
void func_0021ED58(char *p, void *q);
void func_001DC790(char *pch, int i) {
    struct G2 *pg = &D_003DA150;
    unsigned int n = pg->ae[pg->i + i].n;
    int id = -1;
    if (n < 0x2B) id = D_002BCD00[n].id;
    if (id != -1) func_0021ED58(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, id));
    else *pch = 0;
}
