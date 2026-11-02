struct S { int pad[6]; float g18; char pad2[0xAC - 0x1C]; float gAC; };
int FFloatsNear__Ffff(float a, float b, float eps);
/** @sly2-annot addr=0x1510D8
 * Purpose: uZoomTarget < 0 or uZoom ~= uZoomTarget (0.01)
 * Info: proposed name FCpzoomZoomArrived (T2); TU cplcy
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
int func_001510D8(struct S *p)
{
    int f = 0;
    if (p->g18 < 0.0f || FFloatsNear__Ffff(p->gAC, p->g18, 0.01f))
        f = 1;
    return f;
}
