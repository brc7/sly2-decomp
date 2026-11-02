typedef struct OBJ OBJ;
typedef struct { short d; short pad; void (*pfn)(void *, int); } VTE;
typedef struct { char pad[0x103C]; float x; float y; float g; } Q;
struct OBJ {
    char pad[0xC];
    VTE *vt;
    Q *q;
    char pad14[0x28 - 0x14];
    int n;
};
float func_002458F0(float, float);
/** @sly2-annot addr=0x1DA858
 * Purpose: Turn the left stick into an 8-way direction: call screen vt14(dir) when deflection >0.8, vt14(-1)
 *   when back under 0.5
 * Info: proposed name UpdateScreenStickDir (T1); TU screen
 * Notes: names_v2 C hint not used: LoadTnFromBrx__FP2TNP18CBinaryInputStream
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DA858(OBJ *p)
{
    Q *q = p->q;
    int n;
    float g;
    if (q == 0)
        return;
    n = p->n;
    g = q->g;
    if (n == -1) {
        if (0.8f < g) {
            int k = (int)(func_002458F0(q->y, q->x) * 1.2732395f + 8.5f);
            p->vt[14].pfn((char *)p + p->vt[14].d, k % 8);
        }
    } else if (g < 0.5f) {
        p->vt[14].pfn((char *)p + p->vt[14].d, -1);
    }
}
