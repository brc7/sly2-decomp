extern char D_003DD490[];
struct S { char pad[0xEB8]; int a[8]; int n; int b[8]; int c; };
int PloFindSwObject__FP2SWi3OIDP2LO(void *p, int k, int x, struct S *ps);
void func_0020AAA8(struct S *p)
{
    int i;
    p->c = 0;
    for (i = 0; i < p->n; i++) {
        int r = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, p->a[i], p);
        if (r)
            p->b[p->c++] = r;
    }
}
