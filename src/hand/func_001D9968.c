typedef struct VT { struct { short d; short pad; void (*pfn)(void *, int); } a[64]; } VT;
typedef struct { char pad[0xC]; VT *vt; } W;
typedef struct {
    char pad[0xDFC];
    int c;
    int *pn;
    char padE04[0x1300 - 0xE04];
    int cMax;
} Q;
typedef struct {
    char pad[0x20];
    float x20;
    float x24;
    float x28;
    float x2C;
    int x30;
    Q *q;
    W *pw;
    int ids[5];
} OBJ;
extern struct G D_003DD490;
W *func_0015E938(struct G *, int, int, int);
Q *func_001C4840(struct G *);
int func_00132B10(Q *, int);
int func_001441A0(Q *);
void func_00213B00(W *, int);
void func_00212E40(W *, int, float);
/** @sly2-annot addr=0x1D9968
 * Purpose: Health/gadget-power meter (global D_003D9E80): find widget 0x36B (class 0x405), take hero hp
 *   (+0xE00/+0xDFC) and power (MpStepplayerCur/+0x1300) fractions, pick per-character anim ids (Sly
 *   0x33D-341, Murray 0x342-346, Bentley 0x347-34B), set gauge phases 0x34F/0x350
 * Info: proposed name PostHudMeterLoad (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/hud-ui.md#1-widget-hud-objects-hud-tu-0x210eb0-0x21bb88-file-name-unknown,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001D9968(OBJ *p)
{
    Q *q;
    W *pw;
    p->pw = func_0015E938(&D_003DD490, 0x405, 0x36B, 0);
    p->x30 = 0;
    p->q = 0;
    q = func_001C4840(&D_003DD490);
    if (q && func_00132B10(q, 0x34))
        p->q = q;
    if (p->q == 0) {
        p->x20 = p->x24 = 0.0f;
        p->x28 = p->x2C = 0.0f;
        return;
    }
    p->x20 = p->x24 = (float)*p->q->pn / (float)p->q->c;
    p->x28 = p->x2C = (float)func_001441A0(p->q) / (float)p->q->cMax;
    if (func_00132B10(p->q, 0x37)) {
            p->ids[0] = 0x347;
            p->ids[1] = 0x348;
            p->ids[2] = 0x349;
            p->ids[3] = 0x34A;
            p->ids[4] = 0x34B;
    } else if (func_00132B10(p->q, 0x36)) {
            p->ids[0] = 0x342;
            p->ids[1] = 0x343;
            p->ids[2] = 0x344;
            p->ids[3] = 0x345;
            p->ids[4] = 0x346;
    } else {
            p->ids[0] = 0x33D;
            p->ids[1] = 0x33E;
            p->ids[2] = 0x33F;
            p->ids[3] = 0x340;
            p->ids[4] = 0x341;
    }
    pw = p->pw;
    if (pw) {
        pw->vt->a[49].pfn((char *)pw + pw->vt->a[49].d, p->ids[0]);
        func_00213B00(p->pw, 0x34F);
        func_00213B00(p->pw, 0x350);
        func_00212E40(p->pw, 0x34F, 1.0f - p->x20);
        func_00212E40(p->pw, 0x350, 1.0f - p->x28);
    }
}
