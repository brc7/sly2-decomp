struct G;
extern struct G D_003DD490;
struct Z { char pad[0xF8]; int xF8; };
struct Y { char pad[0x74]; int c; struct Z *a[1]; };
struct X { char pad[0xF38]; struct Y *py; };
struct X *PpoHero(struct G *g);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
char *PchzFindSwLangstr__FP2SWi(struct G *g, int id);
void func_0021ED58(char *dst, char *src);
void func_002111F0(char *dst, int i)
{
    struct G *g = &D_003DD490;
    struct X *p = PpoHero(g);
    if (p && FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x34)) {
        struct Y *y = p->py;
        if (y && i < y->c)
            func_0021ED58(dst, PchzFindSwLangstr__FP2SWi(g, y->a[i]->xF8));
    }
}
