typedef unsigned long ulong;
struct DISPU { unsigned int dx : 12; unsigned int dy : 11; unsigned int r0 : 9; unsigned int r1; } __attribute__((aligned(8)));
struct DISPL { ulong dx : 12; ulong dy : 11; ulong rest : 41; };
struct GS { char pad[0x18]; struct DISPU d1; char pad2[0x40 - 0x20]; struct DISPU d2; };
struct S { char pad[0x10]; int x10; };
struct P { char pad[0x8]; void *x8; };
struct G { char pad[0x14C]; float x14C; float x150; };
extern struct G D_003DD040;
extern char D_002BBB60[];
extern float D_002DDF10;
extern float D_002C01C8, D_002C01CC;
extern float D_002F6468, D_002F646C;
extern struct GS D_002DEB80;
int DxSelectionJoy(int);
int DySelectionJoy(int);
float GLimitLm__FP2LMf(void *, float);
void SetSndmgrChannelVolume(struct G *, int, float, int);
void SetWidgetAnimPhase(void *, int, float);
static __inline__ float GClamp(float g, float lo, float hi) { float r; if (g < lo) r = lo; else if (hi < g) r = hi; else r = g; return r; }
/** @sly2-annot addr=0x1AF6D0
 * Purpose: Volume slider items 0x67/0x68 (music/sfx): left/right changes value (DxSelectionJoy),
 *   SetSndmgrChannelVolume, update meter
 * Info: proposed name AdjustPauseVolumeItem (T2); TU menu
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001AF6D0(struct S *s, struct P *p, int *pk)
{
    switch (*pk) {
    case 0x67: {
        float f = (float)DxSelectionJoy(s->x10);
        float g;
        if (f == 0.0f)
            return;
        g = GLimitLm__FP2LMf(D_002BBB60, D_003DD040.x150 + f * D_002DDF10 * 1.5f);
        SetSndmgrChannelVolume(&D_003DD040, 1, g, 1);
        SetWidgetAnimPhase(p->x8, 0x315, g);
        break;
    }
    case 0x68: {
        float f = (float)DxSelectionJoy(s->x10);
        float g;
        if (f == 0.0f)
            return;
        g = GLimitLm__FP2LMf(D_002BBB60, D_003DD040.x14C + f * D_002DDF10 * 1.5f);
        SetSndmgrChannelVolume(&D_003DD040, 0, g, 1);
        SetSndmgrChannelVolume(&D_003DD040, 3, g, 1);
        SetWidgetAnimPhase(p->x8, 0x315, g);
        break;
    }
    case 0x3F: {
        float x = (float)DxSelectionJoy(s->x10);
        float y = (float)DySelectionJoy(s->x10);
        float v;
        int n;
        if (x == 0.0f && y == 0.0f)
            return;
        D_002F6468 = GClamp(D_002F6468 + x * D_002C01C8, 424.0f, 848.0f);
        v = GClamp(D_002F646C + y * D_002C01CC, 10.0f, 90.0f);
        D_002F646C = v;
        {
        struct GS *pgs = &D_002DEB80;
        pgs->d1.dx = pgs->d2.dx = D_002F6468;
        pgs->d1.dy = pgs->d2.dy = (unsigned int)v & ~1;
        }
        break;
    }
    }
}
