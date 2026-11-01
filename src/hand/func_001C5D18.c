struct VTE { short d; short pad; void (*pf)(); };
struct CLOCK { float t, dt; };
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
struct S { char pad[0xC]; struct VTE *vt; char p1[0x1C8 - 0x10]; int x1C8; float x1CC; };
int func_001C4BD8(void *pg, int c, void **ap);
int func_00132B10(struct S *p, int n);
int func_001C5B20(struct S *p, void *pv);
/** @sly2-annot addr=0x1C5D18
 * Purpose: for each collector PZI (CppziCur) check collection
 * Info: proposed name CheckDprizeCollect (T1); TU prize
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C5D18(struct S *p)
{
    void *ap[0x10];
    int i, n;
    if (!p->x1C8) return;
    if (p->x1CC < D_002DDED0.t) {
        ((void (*)(void *, int))p->vt[76].pf)((char *)p + p->vt[76].d, 1);
        return;
    }
    n = func_001C4BD8(D_003DD490, 0x10, ap);
    for (i = 0; ; i++) {
        if (i >= n) break;
        if (i > 0 && !func_00132B10(p, 0x5B)) break;
        if (func_001C5B20(p, ap[i])) break;
    }
}
