typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pf)(); };
struct CLOCK { float t, dt; };
extern struct CLOCK D_002DDED0;
struct LO { char pad[0xC]; struct VTE *vt; char p1[0x28 - 0x10]; unsigned long x28; char p2[0xE8 - 0x30]; int xE8; };
struct G { char pad[0x150]; int x150; };
struct S { char pad[0x8]; struct G *pg; char p0[0x14 - 0xC]; float x14; float x18; char p1[0x70 - 0x1C]; qword x70;
    char p2[0xE0 - 0x80]; struct LO *xE0; char p3[0xEC - 0xE4]; struct VTE *vt; };
void RemoveRip__FP3RIP(struct S *p);
/** @sly2-annot addr=0x1CF268
 * Purpose: RIP default Update (vt entry 5): fade/age and RemoveRip when expired
 * Info: proposed name UpdateRip (T1); TU rip
 * Notes: names_v2 v1 proposed TouchDroplet__FP7DROPLETi (0.167)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void UpdateRip(struct S *p)
{
    struct LO *plo;
    int f;
    if (D_002DDED0.t - p->x14 > p->x18) {
        RemoveRip__FP3RIP(p);
        return;
    }
    if (p->pg->x150 == 1) return;
    plo = p->xE0;
    if (!plo) return;
    if (!plo->xE8) return;
    f = 0;
    if ((int)(plo->x28 >> 60) & 1)
        if (((int (*)(void *, qword))plo->vt[73].pf)((char *)plo + plo->vt[73].d, p->x70)) f = 1;
    ((void (*)(void *, int))p->vt[7].pf)((char *)p + p->vt[7].d, f);
}
