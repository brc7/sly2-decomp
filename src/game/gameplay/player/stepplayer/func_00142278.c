typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct P { char pad[0xC]; struct VTE *vt; char pad2[0x11B0 - 0x10]; char sub[0x10]; void *p11C0; char pad3[0x1290 - 0x11C4]; qword q1290; };
void FadeSoIn(void *p, float f);
qword func_001A62E8(void *p);
void UpdateJtThrowArc(void *p, int n);
void SetJtthrowState(void *p, int n);
/** @sly2-annot addr=0x142278
 * Purpose: Splice: set object a (0x1F7C68 0.25) as carried (+0x11C0) aimed at pos of b (+0x1290),
 *   UpdateJtThrowArc, vt88(7), jtthrow state 7 (0x2206C8)
 * Info: proposed name StartStepplayerThrowAt (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00142278(struct P *p, void *a, void *b)
{
    struct VTE *e;
    FadeSoIn(a, 0.25f);
    p->p11C0 = a;
    p->q1290 = func_001A62E8(b);
    UpdateJtThrowArc(p->sub, 0);
    e = &p->vt[88];
    ((void (*)(void *, int))e->pfn)((char *)p + e->d, 7);
    SetJtthrowState(p->sub, 7);
}
