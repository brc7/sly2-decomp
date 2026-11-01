struct AREQ { int cb; int cbAlign; };
struct E { int x0; int x4; };
struct P { char pad[0x4640]; int c; struct E *ae; };
extern int D_002E1E54;
extern char D_002DD590[];
extern char D_003E1C20[];
int func_00117BD0(void *pbs);
int func_00117D50(void *pbs);
void *func_001AE1D0(struct AREQ *preq);
void func_00118380(void *pbs, int *pn, void *pv, int n);
void func_00118420(void *pbs);
void func_00210BE0(void *pv);
/** @sly2-annot addr=0x13DF80
 * Purpose: Reads localized string table: U8 clang + lang ids, S16 c x {S16 id; clang x string16}; keeps the
 *   g_nLang (D_002E1E54) string into SW.alangstr (+0x4640/+0x4644)
 * Info: proposed name LoadSwLangstrsFromBrx (T1); TU brx_sw
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file,
 *   docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#brx_sw-0x13da18-0x13e368
 * @end-sly2-annot */
void func_0013DF80(struct P *p, void *pbs)
{
    struct AREQ req;
    int i;
    int iMatch = 0;
    int c = func_00117BD0(pbs);
    {
        int j;
        for (j = 0; j < c; j++) {
            if (func_00117BD0(pbs) == D_002E1E54)
                iMatch = j;
        }
    }
    i = 0;
    p->c = func_00117D50(pbs);
    req.cbAlign = 0x10;
    req.cb = p->c * sizeof(struct E);
    p->ae = (struct E *)func_001AE1D0(&req);
    for (; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        int j;
        pe->x0 = func_00117D50(pbs);
        for (j = 0; j < c; j++) {
            if (j == iMatch)
                func_00118380(pbs, &pe->x4, D_002DD590, 0x2E);
            else
                func_00118420(pbs);
        }
    }
    func_00210BE0(D_003E1C20);
}
