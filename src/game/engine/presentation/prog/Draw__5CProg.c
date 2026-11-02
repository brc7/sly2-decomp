typedef int qword __attribute__((mode(TI)));
struct RGBA { unsigned char r, g, b, a; };
struct GS { int a[12]; };
struct PROG { int cRetry; int nRemain; int nTarget; int nMax; char pad[0x50 - 0x10]; int x50; struct RGBA c54, c58, c5C, c60; };
void func_00166920(struct GS *p);
void func_00157330(struct GS *p, int n, qword *buf);
void func_001574B8(struct GS *p);
float GTrunc__Ff(float g);
void FillScreenRect(struct RGBA *pc, struct GS *pgs, float x0, float y0, float x1, float y1);
void func_00166968(struct GS *p);
void func_001576B0(struct GS *p);
void func_001576F0(struct GS *p);
void func_00157348(struct GS *p, int a, int b);
void func_00170278(qword *buf);
/** @sly2-annot addr=0x1C8DD8
 * Purpose: draw progress bar (GIF/DMA packet)
 * Info: proposed name CProg_Draw (T1); class CProg; TU prog
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#prog-0x1c8cc0-0x1c9940
 * @end-sly2-annot */
void Draw__5CProg(struct PROG *p)
{
    qword buf[0x400];
    struct GS gs;
    struct RGBA c;
    int w, s;
    func_00166920(&gs);
    func_00157330(&gs, 0x400, buf);
    func_001574B8(&gs);
    w = (int)((float)p->nTarget * 620.0f / (float)p->nRemain);
    s = (int)((float)w * ((float)(p->nTarget - p->nMax) / (float)p->nTarget));
    c = p->c54;
    FillScreenRect(&c, &gs, 10.0f, GTrunc__Ff(412.909088f) * 1.1f, (float)(s + 10), GTrunc__Ff(412.909088f) * 1.1f + 6.6000004f);
    if (p->x50 <= 0)
        c = p->c58;
    else if (p->x50 == 1)
        c = p->c5C;
    else
        c = p->c60;
    {
        struct RGBA c2 = c;
        FillScreenRect(&c2, &gs, (float)(s + 10), GTrunc__Ff(412.909088f) * 1.1f, (float)(w + 10), GTrunc__Ff(412.909088f) * 1.1f + 6.6000004f);
    }
    func_00166968(&gs);
    func_001576B0(&gs);
    func_001576F0(&gs);
    func_00157348(&gs, 0, 0);
    func_00170278(buf);
}
