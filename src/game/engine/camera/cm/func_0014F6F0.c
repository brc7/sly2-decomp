struct S4 { char a[4]; };
struct CM { char pad[0x24C]; struct S4 s; };
void func_001ED278(void *p);
void RecalcCmFrustrum__FP2CM(struct CM *p);
/** @sly2-annot addr=0x14F6F0
 * Purpose: rgbaFog(+0x24C)=*prgba; 0x1ED278 on it; RecalcCmFrustrum
 * Info: proposed name SetCmRgbaFog (T1); TU cm
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014F6F0(struct CM *p, struct S4 *s)
{
    p->s = *s;
    func_001ED278(&p->s);
    RecalcCmFrustrum__FP2CM(p);
}
