struct VTE { short d; short pad; int (*pfn)(void *, ...); };
struct E { int id; int state; int x; };
struct X150 { char pad[0x30]; int grf; };
struct T { int x0; int x4; int x8; int xC; char pad[0x10]; };
struct P {
    char pad[0xC]; struct VTE *vt; char pad1[0x150 - 0x10]; struct X150 *p150; char pad2[0x1180 - 0x154];
    struct E ae[3]; char pad3[0x1304 - 0x11A4]; float t1304;
};
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern int D_002C3258[];
extern struct T D_002BCD00[];
int func_001C46C0(struct X150 *p, int mask, float g);
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
int func_001441A0(struct P *p);
void func_00141C08(struct P *p, struct E *pe, int n);
/** @sly2-annot addr=0x141A50
 * Purpose: Per frame for the 3 gadget slots (+0x1180, buttons D_002C3258 = 4,1,2): on press check vt99
 *   FCanUseGadget and MP cost (D_002BCD00[id].xC vs MpCur) - error sound 0x2E or start; while held call
 *   vt101, release -> stop
 * Info: proposed name UpdateStepplayerGadgets (T1); TU stepplayer
 * Notes: runtime3/4: 3 gadget slots, buttons {4,1,2}
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); sources: r2b_rest+runtime
 * See: docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00141A50(struct P *p)
{
    int i;
    int *pmask;
    if (!p->p150) return;
    i = 0;
    pmask = D_002C3258;
    do {
        int mask = *pmask;
        struct E *pe = &p->ae[i];
        int id = pe->id;
        switch (pe->state) {
        case 0:
            if (func_001C46C0(p->p150, mask, 0.15f)) {
                if (!p->vt[99].pfn((char *)p + p->vt[99].d, id)) {
                    func_001FB2F8(func_001FBB90(0x2E), 0);
                } else {
                    int n = func_001441A0(p);
                    if (n < D_002BCD00[id].xC) {
                        func_001FB2F8(func_001FBB90(0x2E), 0);
                        p->t1304 = D_002DDED0.t;
                    } else {
                        func_00141C08(p, pe, D_002BCD00[id].x0);
                    }
                }
            }
            break;
        case 1:
            if (!(p->p150->grf & mask) || !((int (*)(void *, int))p->vt[99].pfn)((char *)p + p->vt[99].d, id))
                func_00141C08(p, pe, 0);
            else
                ((void (*)(void *, struct E *))p->vt[101].pfn)((char *)p + p->vt[101].d, pe);
            break;
        }
        pmask++;
    } while (++i < 3);
}
