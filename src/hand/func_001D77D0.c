typedef struct { char pad[0x50]; int c; } LIST;
typedef struct {
    char pad[0x184];
    LIST *plist;
    void *x188;
    char pad18c[0x220 - 0x18C];
    int c;
    void *a[8];
} OBJ;
void *func_001F07E8(void *);
int func_001F0438(LIST *, void *);
void *OidFromSmIsms(LIST *, int);
void SetSmaGoal(void *, void *);
void ReplaceSma(int, OBJ *, int, int, void **);
void func_001D77D0(OBJ *p, int d)
{
    int i0;
    int i;
    if (p->x188 == 0)
        return;
    i = func_001F0438(p->plist, func_001F07E8(p->x188));
    i0 = i;
    for (;;) {
        void *pv;
        int j;
        i += d;
        if (i < 0)
            i = p->plist->c - 1;
        else if (i >= p->plist->c)
            i = 0;
        pv = OidFromSmIsms(p->plist, i);
        for (j = 0; j < p->c; j++) {
            if (p->a[j] == pv) {
                SetSmaGoal(p->x188, pv);
                return;
            }
        }
        if (d == 0)
            d = 1;
        else if (i == i0)
            break;
    }
    ReplaceSma(0, p, -1, 0, &p->x188);
}
