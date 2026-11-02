struct CM { char pad[0x20]; float a, b; int c; };
void RecalcCmFrustrum__FP2CM(struct CM *p);
/** @sly2-annot addr=0x14F5A0
 * Purpose: fov(+0x20)=fovTarget(+0x24)=f, dfov=0; RecalcCmFrustrum if changed > 1e-4
 * Info: proposed name SetCmFov (T1); TU cm
 * Confidence: high [T1 0.90] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmFov__FP2CMf(struct CM *p, float f)
{
    int fChanged = __builtin_fabsf(f - p->a) > 0.0001f;
    p->a = f;
    p->b = f;
    p->c = 0;
    if (fChanged) RecalcCmFrustrum__FP2CM(p);
}
