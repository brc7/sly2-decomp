typedef struct { char *pch; char *pchOut; } ENT;
typedef struct { int c; ENT *a; int x8; int xC; } TBL;
typedef struct { int a[4]; } ALLOC;
extern struct G { char pad[0x4640]; int f; } D_003DD490;
extern ALLOC *func_00116A98(ALLOC *, int, int);
extern char *PchzFindSwLangstr__FP2SWi(struct G *, char *);
extern int func_0021ED28(char *);
extern char *func_001169D8(ALLOC *, char *, int);

/** @sly2-annot addr=0x210BE0
 * Purpose: For each entry look up localized string via g_sw (0x13E368) and copy into a linear heap
 * Info: proposed name LocalizeSwStringTable (T2); TU sw
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210BE0(TBL *p)
{
    ALLOC al;
    int i;
    if (D_003DD490.f == 0)
        return;
    func_00116A98(&al, p->x8, p->xC);
    for (i = 0; i < p->c; i++) {
        ENT *e = &p->a[i];
        e->pchOut = PchzFindSwLangstr__FP2SWi(&D_003DD490, e->pch);
        if (func_0021ED28(e->pchOut))
            e->pchOut = func_001169D8(&al, e->pchOut, 4);
        else
            e->pchOut = 0;
    }
}
