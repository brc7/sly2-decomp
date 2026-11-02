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
/** @sly2-annot addr=0x1D77D0
 * Purpose: Move SMA goal by d through SM states skipping disallowed ones (wrap)
 * Info: proposed name StepCharSelect (T2); TU tu_1D7238
 * Notes: names_v2 C proposed BindSgg__FP3SGG (0.638)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/player.md#tu_1d7238-0x1d7238-0x1d7a30
 * @end-sly2-annot */
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
