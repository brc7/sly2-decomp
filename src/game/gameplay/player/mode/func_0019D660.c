struct O;
typedef struct { short d; short pad; void (*pfn)(void *, ...); } VTE;
struct O { char pad[0xC]; VTE *vt; char pad2[0x58 - 0x10]; int x58; };
struct B { char pad[0x1A8]; float x1a8; };
struct A { struct B *x0; char pad[0x1C]; int x20; char pad2[0x18]; float x3c; };
struct S { struct A *x0; int x4; struct O *x8; int xc; struct O *x10; void *x14; };
extern char D_002DEB30[];
extern char D_004435F0[];
void func_0019D948(struct S *p);
void BroadcastGameEvent(void *p, int n, int m);
void PlayWidgetAnim(struct O *p, int n);
void SetWidgetAnimPhase(struct O *p, int n, float f);
void func_00227788(void *p);
void SetReconSelState(void *p, int n);
void SetWidgetState(struct O *p, int n);
void SetWidgetRequest(struct O *p, int n);
/** @sly2-annot addr=0x19D660
 * Purpose: Set HUD state: on first use gather tags (0x19D948); state 0 broadcasts game event 0xC for kind 1,
 *   starts timers/speaker labels
 * Info: proposed name SetJtModeHudState (T2); TU mode
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#mode-0x19cbf8-0x19de10
 * @end-sly2-annot */
void func_0019D660(struct S *p, int st)
{
    int old;
    if (p->x4 == st) return;
    if (p->x4 == -1) func_0019D948(p);
    old = p->x4;
    p->x4 = st;
    switch (st) {
    case 0:
        if (p->x0->x20 == 1) BroadcastGameEvent(D_002DEB30, 0xC, 0);
        else BroadcastGameEvent(D_002DEB30, 7, 0);
        if (p->x8) {
            p->x8->vt[49].pfn((char *)p->x8 + p->x8->vt[49].d, -1);
            PlayWidgetAnim(p->x8, 0x352);
            PlayWidgetAnim(p->x8, 0x351);
            SetWidgetAnimPhase(p->x8, 0x352, (p->x0->x0->x1a8 + 3.1415927f) * 0.15915494f);
            SetWidgetAnimPhase(p->x8, 0x351, p->x0->x3c);
        }
        if (p->x0->x20 == 1) {
            func_00227788(D_004435F0);
            if (p->x10) p->x10->vt[49].pfn((char *)p->x10 + p->x10->vt[49].d, -1);
        }
        if (p->x14) SetReconSelState(p->x14, 0);
        break;
    case 2:
        if (p->x0->x20 == 1) BroadcastGameEvent(D_002DEB30, 0xD, 0);
        else BroadcastGameEvent(D_002DEB30, 8, 0);
        if (p->x8) p->x8->vt[50].pfn((char *)p->x8 + p->x8->vt[50].d);
        if (p->x10) p->x10->vt[50].pfn((char *)p->x10 + p->x10->vt[50].d);
        if (p->x14) SetReconSelState(p->x14, 10);
        break;
    case -1:
        if (old != 2) {
            if (p->x0->x20 == 1) BroadcastGameEvent(D_002DEB30, 0xD, 0);
            else BroadcastGameEvent(D_002DEB30, 8, 0);
        }
        if (p->x8 && p->x8->x58) {
            SetWidgetState(p->x8, 1);
            SetWidgetRequest(p->x8, 1);
        }
        if (p->x10 && p->x10->x58) {
            SetWidgetState(p->x10, 1);
            SetWidgetRequest(p->x10, 1);
        }
        if (p->x14) SetReconSelState(p->x14, -1);
        break;
    }
}
