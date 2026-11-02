typedef int qword __attribute__((mode(TI)));
extern struct { float t; float dt; } D_002DDED0;
typedef struct { int k; int pad[7]; qword q; int pad2[4]; } MSG;
struct TGT;
typedef struct { short d; short pad; void (*pf)(struct TGT *, MSG *); } VTE;
typedef struct TGT { char pad[0xC]; VTE *vt; } TGT;
typedef struct {
    char pad0[0x70];
    qword x70;
    char pad80[0xF0 - 0x80];
    TGT *xF0;
    float xF4;
    float xF8;
} OBJ;
void UpdateRip(OBJ *);
/** @sly2-annot addr=0x1D1160
 * Purpose: REMIT Update: UpdateRip; accumulate rate*dt and send emit message (4) with position to target
 * Info: proposed name UpdateRemit (T1); TU rip
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void UpdateRemit(OBJ *p)
{
    UpdateRip(p);
    if (p->xF0) {
        p->xF8 += D_002DDED0.dt * p->xF4;
        while (p->xF8 >= 1.0f) {
            MSG m;
            TGT *t;
            m.k = 4;
            m.q = p->x70;
            t = p->xF0;
            t->vt[51].pf((TGT *)((char *)t + t->vt[51].d), &m);
            p->xF8 -= 1.0f;
        }
    }
}
