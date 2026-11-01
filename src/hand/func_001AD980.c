struct G { char pad[0x3A18]; char x3A18[1]; };
extern struct G D_003DD490;
extern char D_003DA530[], D_002A6EC8[], D_003D4A60[];
int func_001D9880(void *p);
void *func_00210B28(void);
int func_001DDD40(void *p);
char *PchzFindSwLangstr__FP2SWi(void *p, int n);
char *func_001D82B8(void *p, void *q);
void func_0021ECD8(char *buf, char *fmt, ...);
void func_001AD980(char *buf)
{
    char *a;
    struct G *g;
    char *fmt;
    if (func_001D9880(D_003DA530))
        return;
    fmt = D_002A6EC8;
    g = &D_003DD490;
    a = PchzFindSwLangstr__FP2SWi(g, func_001DDD40(func_00210B28()));
    func_0021ECD8(buf, fmt, a, func_001D82B8(g->x3A18, D_003D4A60));
}
