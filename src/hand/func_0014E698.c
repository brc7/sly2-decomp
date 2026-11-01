void SetVectorCylind(void *a, void *b, float x, float y, float z);
/** @sly2-annot addr=0x14E698
 * Purpose: dposAdjust = cylindrical vector (radHome+PI, sAdjust) from CPDEFI +0x30/+0x34 via 0x11E1A0
 * Info: proposed name CalcCmAdjust (T1); TU cm
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014E698(void *a, char *b)
{
    SetVectorCylind(a, b, *(float *)(b + 0x30) + 3.14159265f, *(float *)(b + 0x34), 0.0f);
}
