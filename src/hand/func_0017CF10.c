struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002DFB7C;
struct O { char pad[0x198]; float g198; char pad2[0x14]; char a1B0[0xF84 - 0x1B0]; int nF84; float gF88; int nF8C; };
struct S { char pad[0x10]; struct O *p10; char pad2[0x1C]; int n30; int pad3; float g38; float g3C; };
struct L { float a, b, c; };
void func_001EEF98(struct S *p);
int func_0017D188(struct S *p);
void func_0017D058(struct S *p, int n);
float func_0011D018(float a, float b, float dt, struct L *pl, int n);
float func_001E66C8(int n, float g);
void func_001C3500(int a, float g, int b, void *pv);
/** @sly2-annot addr=0x17CF10
 * Purpose: BHVPIPE Update: shimmy along the pipe toward the goal with smoothed speed; keep the pipe goal
 *   matrix
 * Info: proposed name UpdateBhvpipe (T1); class BHVPIPE slot10; TU guardbhv
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017CF10(struct S *p) {
    int s;
    func_001EEF98(p);
    do {
        s = p->n30;
        if (s == 0) {
            if (func_0017D188(p)) {
                func_0017D058(p, 1);
            } else {
                struct L l;
                float g;
                p->p10->nF8C = p->p10->gF88 < p->g38;
                if (p->p10->nF8C) D_002DFB7C = -p->p10->g198;
                else D_002DFB7C = p->p10->g198;
                {
                    float d = D_002DFB7C;
                    l.b = 0.0f;
                    l.c = 0.1f;
                    l.a = __builtin_fabsf(d);
                    g = func_0011D018(p->g3C, d, D_002DDED0.dt, &l, 0);
                }
                p->g3C = g;
                p->p10->gF88 = func_001E66C8(p->p10->nF84, p->p10->gF88 + g * D_002DDED0.dt);
            }
        }
    } while (p->n30 != s);
    {
        struct O *o = p->p10;
        func_001C3500(o->nF84, o->gF88, o->nF8C, o->a1B0);
    }
}
