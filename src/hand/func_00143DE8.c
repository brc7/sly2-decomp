struct VTE { short d; short pad; void (*pfn)(void *, ...); };
struct LO { char pad[0xC]; struct VTE *vt; char pad2[0x40 - 0x10]; int *p40; };
struct P {
    char pad[0xC]; struct VTE *vt; char pad1[0x26C - 0x10]; char m26C[0x4C8 - 0x26C]; int x4C8;
    char pad2[0xF00 - 0x4CC]; char mF00[0x1084 - 0xF00]; struct LO *plo; char pad3[0x11B4 - 0x1088]; int x11B4;
};
void func_001EECD0(void *p);
int func_001443D8(struct P *p);
void func_0019CD38(void *p, int n);
/** @sly2-annot addr=0x143DE8
 * Purpose: Set object at +0x1084: old one gets vt2, leaves grab states 5/6 via vt88(0); new one gets vt1,
 *   message list cleared, jtmode reset
 * Info: proposed name SetStepplayerRide (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00143DE8(struct P *p, struct LO *plo)
{
    struct LO *old = p->plo;
    if (plo == old) return;
    if (old) {
        old->vt[2].pfn((char *)old + old->vt[2].d);
        switch (p->x4C8) {
        case 5: case 6:
            p->vt[88].pfn((char *)p + p->vt[88].d, 0);
            break;
        }
    }
    p->plo = plo;
    if (plo) {
        plo->vt[1].pfn((char *)plo + plo->vt[1].d);
        func_001EECD0(p->m26C);
        if (func_001443D8(p)) func_0019CD38(p->mF00, -1);
        if (p->x11B4 != -1 && *p->plo->p40 != 0x1E)
            p->vt[88].pfn((char *)p + p->vt[88].d, 0);
    }
}
