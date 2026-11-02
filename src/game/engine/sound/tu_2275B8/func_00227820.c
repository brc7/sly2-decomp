struct P { int id; };
struct Q { void *p0; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern char D_003DD040[];
extern char D_002A97F0[];
float func_001FCFD0(void *p, int id);
int CRichText_Cch(struct Q *q);
int func_001FE848(void *p, int id);
float func_001FE888(void *p);
void CRichText_Trim(struct Q *q, int n);
void func_0021EDA0(void *p, void *s);
/** @sly2-annot addr=0x227820
 * Purpose: draws text label via blot helpers and strcpy
 * Info: proposed name RenderSpeakerText (T2); TU tu_2275B8
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/sound.md#tu_2275b8-0x2275b8-0x227bb8
 * @end-sly2-annot */
void func_00227820(struct P *p, struct Q *q)
{
    void *pv;
    float f;
    int c, n, fShow;
    if (p->id == -1) return;
    pv = D_003DD040;
    f = func_001FCFD0(pv, p->id) - 0.5f;
    c = CRichText_Cch(q);
    if (f <= 0.0f || !func_001FE848(pv, p->id))
        n = c;
    else
        n = (int)(func_001FE888(pv) / f * (float)c);
    if (n < c) {
        CRichText_Trim(q, n);
        fShow = 1;
    } else {
        fShow = (int)(D_002DDED0.t * 4.0f) & 1;
    }
    if (fShow) func_0021EDA0(q->p0, D_002A97F0);
}
