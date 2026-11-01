struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; char p1[0x150 - 0x10]; void *x150; };
struct G { char pad[0x3EB8]; int c; struct LO *a[16]; char p1[0x3EFC - 0x3EFC]; struct LO *plo3EFC; };
extern struct G D_003DD490;
int GetPoCtlState(struct LO *p);
void *func_001AE4F8(int cb);
void *POCTL_ctor(void *pv, struct LO *p);
void SwitchToPo(struct G *pg, struct LO *plo);
void SetPoctlState(void *pv, int n);
/** @sly2-annot addr=0x1C3ED8
 * Purpose: PO virtual slot 79 (Lilly SetCtlState): on first use register PO in g_sw PO list (+0x3EB8, max 16)
 *   and allocate its POCTL (+0x150, 0xE0); state 1 makes it the current PO (old one gets SetCtlState(0)),
 *   0/-1 drop it; then POCTL::SetState
 * Info: proposed name SetPoCtlState (T1); class PO slot79; TU po
 * Notes: names_v2 B proposed DestroyedRobRoc__FP3ROBP3ROC (0.856)
 * Confidence: high [T1 0.80] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50,
 *   docs/reference/functions/player.md#tu_1d7238-0x1d7238-0x1d7a30
 * @end-sly2-annot */
void SetPoCtlState(struct LO *p, int n)
{
    if (n == GetPoCtlState(p)) return;
    if (!p->x150) {
        struct G *pg = &D_003DD490;
        if ((unsigned int)pg->c >= 16) return;
        pg->a[pg->c++] = p;
        p->x150 = POCTL_ctor(func_001AE4F8(0xE0), p);
    }
    switch (n) {
    case 1: {
        struct G *pg = &D_003DD490;
        struct LO *plo = pg->plo3EFC;
        if (p != plo) {
            if (plo) ((void (*)(void *, int))plo->vt[79].pf)((char *)plo + plo->vt[79].d, 0);
            SwitchToPo(pg, p);
        }
        break;
    }
    case -1:
    case 0: {
        struct G *pg = &D_003DD490;
        if (p == pg->plo3EFC) SwitchToPo(pg, 0);
        break;
    }
    }
    SetPoctlState(p->x150, n);
}
