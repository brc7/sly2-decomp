struct CMX { char pad[0x300]; int n; float a; float b; };
/** @sly2-annot addr=0x14D8D8
 * Purpose: Cut with explicit pan/tilt: radCutPan(+0x304)/radCutTilt(+0x308) = args, nCut = max(nCut,2)
 * Info: proposed name SetCmCut (T1); TU cm
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmCut(struct CMX *p, float a, float b)
{
    int m = 2;
    p->b = b;
    p->a = a;
    if (m < p->n) m = p->n;
    p->n = m;
}
