struct F { int v; char pad[0x4C]; };
struct E { int pad[3]; int iFirst; char p2[0x68 - 0x10]; struct F *af; };
struct A { int id; int pad[8]; int f24; };
struct S { char pad[0x3C]; struct A *pa; };
extern struct E *D_003E1398;
int IsrcFromIdProxy__Fi(int id);
int IcopyFromIdProxy(int id);
/** @sly2-annot addr=0x1CB658
 * Purpose: per-copy value from aproxysrc[isrc] table for the object's proxy id
 * Info: proposed name GetProxyCopyValue (T2); TU proxy
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
int func_001CB658(struct S *p)
{
    struct A *pa = p->pa;
    int i, j;
    struct E *pe;
    struct F *pf;
    if (!pa || !pa->f24) return 0;
    i = IsrcFromIdProxy__Fi(pa->id);
    j = IcopyFromIdProxy(p->pa->id);
    pe = &D_003E1398[i];
    pf = &pe->af[j - pe->iFirst];
    return pf->v;
}
