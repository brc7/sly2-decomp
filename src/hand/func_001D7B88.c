struct MSG { int cb; int nKind; };
typedef struct BIS BIS;
typedef struct { int c; void *pv; char ht[1]; } OBJ;
extern char D_002DD590[];
extern struct G D_003D4A60;
int func_00117C20(BIS *);
void func_00117968(BIS *, int, void *);
void CBinaryInputStream_ReadStringSw(BIS *, char **, void *, int);
void func_001D8600(void *, int);
void func_001D8690(void *, char *, int);
void *func_001AE1D0(struct MSG *);
void func_001D7C80(OBJ *, struct G *);
void func_001D7B88(OBJ *p, BIS *pbis)
{
    struct MSG msg;
    char *pchz;
    int i;
    int n;
    int c;
    n = func_00117C20(pbis);
    func_001D8600(p->ht, n);
    for (i = 0; i < n; i++) {
        int v = func_00117C20(pbis);
        CBinaryInputStream_ReadStringSw(pbis, &pchz, D_002DD590, 0x2D);
        func_001D8690(p->ht, pchz, v);
    }
    c = func_00117C20(pbis);
    p->c = c;
    msg.cb = c * 10;
    msg.nKind = 0x10;
    p->pv = func_001AE1D0(&msg);
    func_00117968(pbis, c * 10, p->pv);
    func_001D7C80(p, &D_003D4A60);
}
