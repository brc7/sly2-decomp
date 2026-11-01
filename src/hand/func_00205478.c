extern char D_003DD040[];
extern char D_003DD490[];
struct S { char pad[0x2A0]; int a[6]; };
int func_001FD0C0(void *p, int i);
void *PloFindSwObject__FP2SWi3OIDP2LO(void *p, int a, int b, struct S *ps);
void *func_001F03C8(void *x, struct S *ps);
void func_001F0198(void *x, struct S *ps, int a, int b, void **ppv);
void func_00205478(struct S *p, int mask)
{
    int i;
    for (i = 0; i < 6; i++) {
        int f = (unsigned int)i < 6;
        int bit = 1 << i;
        if (!f) bit = 0;
        if (mask & bit) {
            if (++p->a[i] < 2) {
                int id = func_001FD0C0(D_003DD040, i);
                void *x = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, id, p);
                if (x) {
                    void *y = func_001F03C8(x, p);
                    if (!y)
                        func_001F0198(x, p, -1, 9, &y);
                }
            }
        }
    }
}
