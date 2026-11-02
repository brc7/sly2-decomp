typedef struct RGBA { unsigned char r, g, b, a; } RGBA;

/** @sly2-annot addr=0x11DCD8
 * Purpose: RGBA bytes -> floats /255
 * Info: proposed name ConvertRgbaToVector (T2); TU util
 * Notes: names_v2 v1 proposed Tex0FromTexIframeCtk__FP3TEXi3CTK (0.004)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
void func_0011DCD8(RGBA *prgba, float *pg)
{
    pg[0] = prgba->r * (1.0f / 255.0f);
    pg[1] = prgba->g * (1.0f / 255.0f);
    pg[2] = prgba->b * (1.0f / 255.0f);
}
