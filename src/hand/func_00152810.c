struct E { int oid; int k; void *o; int kk; float ag[3]; };
struct S { char pad[0x234]; float g234; float g238; char pad2[0x2A8 - 0x23C]; int c; struct E ae[8]; };
extern char D_003DD490[];
void *func_0015E938(void *g, int k, int oid, void *p);
void func_001F1448(void *o2, void *o, int k, int n, void **ppo);
int func_00152628(int k);
float func_0011D448(float a, float b);
/** @sly2-annot addr=0x152810
 * Purpose: Callback: for each entry find object by oid and its child 0x625, bind with 0x1F1448, pick 3 random
 *   grade thresholds across the score range
 * Info: proposed name BindRhythmMgrGrades (T2); TU rhythm
 * Confidence: low (draft) [T2 0.35] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_00152810(struct S *p)
{
    int i, j;
    float d = __builtin_fabsf(p->g238 - p->g234) / 3.0f;
    float h = d * 0.5f;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        void *o = func_0015E938(D_003DD490, 4, pe->oid, p);
        void *o2;
        float g;
        if (!o)
            continue;
        o2 = func_0015E938(D_003DD490, 1, 0x625, o);
        if (!o2)
            continue;
        func_001F1448(o2, o, pe->k, 0, &pe->o);
        pe->kk = func_00152628(pe->k);
        g = p->g238;
        for (j = 0; j < 2; j++) {
            g -= d;
            pe->ag[j] = func_0011D448(g - h, g + h);
        }
        pe->ag[2] = p->g234;
    }
}
