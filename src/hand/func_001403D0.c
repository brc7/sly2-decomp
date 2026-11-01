struct VTE { short d; short pad; void (*pfn)(void *, void *); };
struct O { int pad[3]; struct VTE *vt; };
struct Q { char pad[0x80]; int x80; };
struct G { char pad[0x10B4]; int x10B4; };
struct P {
    char pad[0x150]; struct Q *x150;
    char pad1[0x26C - 0x154]; int x26C; int c270; struct O *a274[1];
    char pad2[0xDF4 - 0x278]; int xDF4;
    char pad3[0xEFC - 0xDF8]; int xEFC;
    char pad4[0xFD0 - 0xF00]; int xFD0;
    char pad5[0x1084 - 0xFD4]; void *x1084;
};
extern struct G D_002DFC00;
extern char D_002DE2E0[];
extern char D_003DA0F0[];
void func_00203FC0(struct P *p);
void func_00141A50(struct P *p);
int func_001C46C0(struct Q *q, float g, int n);
int func_001C4660(struct Q *q, float g, int n);
void func_00140338(struct P *p);
void func_00144640(int *pl);
int func_001449C8(int *pl);
void func_001E46A0(void *p, struct Q *q);
void func_00140558(struct P *p);
void func_001A2460(struct P *p);
void func_00140750(struct P *p);
void func_00142AF8(struct P *p);
int func_00131968(int x);
void func_001306A8(int *px);
int func_0014FD50(void *p);
void func_00187DE0(struct G *pg, int n);
void func_001DB490(void *p);
void func_001EED10(int *p);
/** @sly2-annot addr=0x1403D0
 * Purpose: STEPPLAYER vt16 per-frame: STEP update 0x203FC0, input/actions, vehicle/ride component (+0xFD0),
 *   interact-target pick 0x142AF8, gadget/menu button handling when the default camera policy is active
 *   (FActiveCplcy(g_cm+0x3A0))
 * Info: proposed name UpdateStepplayer (T1); class STEPPLAYER slot16; TU stepplayer
 * Notes: runtime6 P1: calls the top BHV slot 14 with the POCTL (AI partner, S62)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r2_131F70+runtime
 * See: docs/engine/player.md#8-look-mode-recon-mode-and-the-ai-partner,
 *   docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001403D0(struct P *p)
{
    int *pl;
    func_00203FC0(p);
    func_00141A50(p);
    if (func_001C46C0(p->x150, 0.0f, 0x200))
        func_00140338(p);
    pl = &p->xFD0;
    if (p->xDF4 == -1) {
        func_00144640(pl);
        if (!func_001449C8(pl)) {
            if (p->c270 > 0) {
                struct O *po = p->a274[p->c270 - 1];
                struct VTE *e = &po->vt[14];
                e->pfn((char *)po + e->d, p->x150);
            } else if (p->x1084) {
                func_001E46A0(p->x1084, p->x150);
            }
        }
    }
    func_00140558(p);
    func_001A2460(p);
    func_00140750(p);
    func_00142AF8(p);
    if (p->xEFC && func_00131968(p->xEFC))
        func_001306A8(&p->xEFC);
    if (!func_0014FD50(D_002DE2E0) && p->x150->x80 == 1 && func_001C4660(p->x150, 0.0f, 0x100)) {
        struct G *pg = &D_002DFC00;
        if ((pg->x10B4 & 0x108) == 0x100) {
            func_00187DE0(pg, 0x100);
            func_001DB490(D_003DA0F0);
        }
    }
    if (!func_001449C8(pl))
        func_001EED10(&p->x26C);
}
