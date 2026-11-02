extern struct CLOCK { float a[16]; } D_002DDED0;
struct P { int a, b; };
struct S { char p0[0x40]; int n40; float g44; int pad; struct P ap[64]; char p1[0x25C-0x24C]; int n25C, n260, n264; };
/** @sly2-annot addr=0x1DB820
 * Purpose: ThiefNet: state -1, clear the 64-entry item list and anim handles
 * Info: proposed name ClearThiefnet (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DB820(struct S *p) {
    unsigned int i;
    p->n40 = -1;
    p->g44 = D_002DDED0.a[15];
    p->n25C = -1;
    p->n260 = -1;
    p->n264 = -1;
    for (i = 0; i < 64; i++) {
        p->ap[i].a = -1;
        p->ap[i].b = -1;
    }
}
