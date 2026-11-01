typedef struct S { char pad[0x1C]; int c; int pad2[2]; unsigned short i; short pad3; int fDirty; int x30; int x34; } S;
int FIsSlEmpty(void *p);
static inline int Clamp(int n, int lo, int hi)
{
    if (n < lo) return lo;
    if (n > hi) return hi;
    return n;
}
void func_001EDA68(S *p, int n)
{
    unsigned short i;
    if (FIsSlEmpty(&p->x34)) return;
    i = Clamp(n, 0, p->c - 1);
    if (p->i != i) {
        p->i = i;
        p->fDirty = 1;
    }
}
