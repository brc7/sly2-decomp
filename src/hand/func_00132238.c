struct K { int a; int b; };
extern struct K D_002A4790;
extern char D_003DD490[];
struct S { void *pframe; };
void func_001F4FB0(struct S *);
void *func_00127DB8(void *, int, int);
void EnsureSwCallback(void *, void *, struct K, int);
void func_00132238(struct S *p) {
    func_001F4FB0(p);
    if (p->pframe && func_00127DB8(p->pframe, 5, 1)) EnsureSwCallback(D_003DD490, p, D_002A4790, 0);
}
