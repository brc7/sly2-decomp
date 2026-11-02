/* float g placed after the pointer params: its param copy comes after pgMax, lengthening pgMax so the hoisted 0x40 wins $fp in global alloc */
typedef int qword __attribute__((mode(TI)));
struct T { int a, b, c; };
struct O { char pad[0x70]; int c; struct T *at; };
struct HIT { char pad[0x40]; float g40; float g44; char pad2[0x18]; };
int func_00118668(struct T *pBegin, struct T *pEnd, qword pos, qword dir, int c, struct HIT *ahit);
int ClsgMergeAlsg(int c, struct HIT *ahit, int cNew, struct HIT *ahitNew, int cMax, struct HIT *ahitOut, int f);
/** @sly2-annot addr=0x14EA08
 * Purpose: Clips segment pos->dir against each SO's camera BSP (0x118668), merges intervals (0x14C9F8),
 *   returns first clear gap > duMin in *puFrom/ *puTo
 * Info: proposed name FFindCmClearInterval (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
int FFindCmClearInterval(int n, struct O **apo, qword pos, qword dir, float *pgMin, float *pgMax, float g)
{
    struct HIT ahit[0x40];
    int c = 0;
    int i;
    int j;
    float g0, g1;
    for (i = 0; i < n; i++) {
        struct O *po = apo[i];
        int cNew = func_00118668(po->at, po->at + po->c, pos, dir, 0x40 - c, &ahit[c]);
        if (cNew)
            c = ClsgMergeAlsg(c, ahit, cNew, &ahit[c], 0x40, ahit, 0);
    }
    for (j = 0; ; j++) {
        if (c < j)
            return 0;
        g0 = 0.0f;
        if (j > 0)
            g0 = ahit[j - 1].g44;
        if (j < c)
            g1 = ahit[j].g40;
        else
            g1 = 1.0f;
        if (g < g1 - g0)
            break;
    }
    if (pgMin)
        *pgMin = g0;
    if (pgMax)
        *pgMax = g1;
    return 1;
}
