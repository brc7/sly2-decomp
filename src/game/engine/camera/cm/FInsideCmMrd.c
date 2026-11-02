typedef int qword __attribute__((mode(TI)));
struct P { int pad[7]; float g1C; };
/** @sly2-annot addr=0x14F898
 * Purpose: True if |dpos| < s*rCullScale; *pu = fade fraction between outer radius and (r-400*rCullScale)
 * Info: proposed name FInsideCmMrd (T1); TU cm
 * Notes: names_v2 v1 proposed FUN_00145de8 (0.000)
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/rendering.md#10-models-globset-at-render-time-cstatic,
 *   docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
int FInsideCmMrd(struct P *p, qword v, float g, float *pu)
{
    qword l, one;
    float l2, r, r2, d2;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(v));
    r = g * p->g1C;
    __asm__("qmfc2.ni %0, %1" : "=r"(l2) : "j"(l));
    r2 = r * r;
    if (r2 < l2)
        return 0;
    d2 = r - p->g1C * 400.0f;
    d2 = d2 * d2;
    if (d2 < l2)
        *pu = (r2 - l2) / (r2 - d2);
    else
        *pu = 1.0f;
    return 1;
}
