struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
struct X;
struct X {
    char pad[0x40]; int fl;
    char pad1[0x68 - 0x44]; struct X *f68;
    char pad2[0x94 - 0x6C]; int c94; struct X **a98;
    char pad3[0xA0 - 0x9C]; int cA0; struct X **aA4;
    char pad4[0xB0 - 0xA8]; int fB0; int fB4; struct O *pB8; struct O *pBC; struct X *fC0; int fC4; float fC8;
};
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
int FTaskComplete(struct X *p);
void SetLoBusyk(struct X *p, int n);
void IncrementSwHandsOff(void *pg, int n);
void DecrementSwHandsOff(void *pg, int n);
void func_00213B00(struct O *po, int n);
static __inline__ void VCall50(struct O *po)
{
    struct VTE *e = &po->vt[50];
    ((void (*)(void *))e->pfn)((char *)po + e->d);
}
static __inline__ void VCall49(struct O *po, int n)
{
    struct VTE *e = &po->vt[49];
    ((void (*)(void *, int))e->pfn)((char *)po + e->d, n);
}
/** @sly2-annot addr=0x163170
 * Purpose: Sets completion phase (0 none,1 intro,2 outro,3 shared widget): entering from 0 finds the next
 *   task, busy-flags the LO and increments hands-off; phase 0 hides widgets and decrements hands-off
 * Info: proposed name SetJobCompletionPhase (T1); TU game
 * Notes: runtime6 G8: SetJobCompletionPhase(1): IncrementSwHandsOff(SW,2) + WIDGET128 banner (S61)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources: r3b_rest+runtime
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void SetJobCompletionPhase(struct X *p, int n)
{
    int i;
    if (p->fC4 == n) return;
    if (p->fC4 == 0) {
        int v;
        p->fC0 = 0;
        for (i = 0; i < p->f68->cA0; i++) {
            struct X *px = p->f68->aA4[i];
            if (px->fC4) {
                p->fC0 = px;
                break;
            }
        }
        if (p->fC0) {
            int j;
            for (j = 0; j < p->fC0->c94; j++) {
                struct X *px = p->fC0->a98[j];
                if (px != p->f68 && !FTaskComplete(px)) {
                    p->fC0 = 0;
                    break;
                }
            }
        }
        SetLoBusyk(p, 1);
        v = p->fl;
        if ((v ^ 1) & 1)
            *(int *)v |= 0x102;
        else
            p->fl = v | 0x102;
        IncrementSwHandsOff(D_003DD490, 2);
    }
    p->fC4 = n;
    p->fC8 = D_002DDED0.t;
    switch (n) {
    case 0: {
        int v;
        if (p->pBC) VCall50(p->pBC);
        if (p->pB8) VCall50(p->pB8);
        SetLoBusyk(p, 2);
        v = p->fl;
        if ((v ^ 1) & 1)
            *(int *)v &= ~0x102;
        else
            p->fl = v & ~0x102;
        DecrementSwHandsOff(D_003DD490, 2);
        break;
    }
    case 1:
        VCall49(p->pB8, -1);
        func_00213B00(p->pB8, p->fB0);
        break;
    case 3:
        VCall49(p->pBC, -1);
        break;
    case 2:
        func_00213B00(p->pB8, p->fB4);
        break;
    }
}
