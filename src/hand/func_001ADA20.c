struct CRef { int m_tagk; float m_tag; };
struct E { int x0, x4, x8, xC, x10; int x14; int x18; };
struct G { int pad[9]; int x24; };
struct GD { char pad[0x3B1C]; int x3B1C; };
struct B { char data[0x5464]; };
extern struct G D_003D9ED0;
extern struct E D_003D9EDC[];
extern struct GD D_003DD490;
extern struct B D_002E6604[];
int func_001B0108(struct G *g);
int func_001B0078(struct G *g);
void func_002581C0(struct CRef *ret, int a, void *b);
void func_00129EC8(struct CRef *);
char *PchzFindSwLangstr__FP2SWi(void *p, int n);
char *func_0021ED58(char *d, const char *s);
char *func_0021EDA0(char *d, const char *s);
void func_001ADA20(char *buf)
{
    struct G *g = &D_003D9ED0;
    struct GD *gd;
    struct CRef ref;
    float f;
    if (!func_001B0108(g))
        return;
    gd = &D_003DD490;
    {
    struct E *e = &D_003D9EDC[g->x24];
    func_002581C0(&ref, gd->x3B1C, &D_002E6604[e->x14]);
    }
    f = ref.m_tag;
    func_00129EC8(&ref);
    switch (func_001B0078(g)) {
    case 0x1E:
        if (f != 0.0f)
            func_0021ED58(buf, PchzFindSwLangstr__FP2SWi(gd, 0x25));
        else
            func_0021ED58(buf, PchzFindSwLangstr__FP2SWi(gd, 0x26));
        break;
    case 0x1F:
        if (f != 0.0f)
            func_0021ED58(buf, PchzFindSwLangstr__FP2SWi(gd, 0x27));
        else
            func_0021ED58(buf, PchzFindSwLangstr__FP2SWi(gd, 0x26));
        break;
    case 0x19:
        func_0021ED58(buf, PchzFindSwLangstr__FP2SWi(gd, 0x29));
        if (f != 0.0f) {
            func_0021EDA0(buf, "\n");
            func_0021EDA0(buf, PchzFindSwLangstr__FP2SWi(gd, 0x25));
        }
        break;
    }
}
