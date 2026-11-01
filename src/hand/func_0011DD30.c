typedef struct VECTOR { float x, y, z; } VECTOR;
typedef struct RGBA { unsigned char r, g, b, a; } RGBA;

/** @sly2-annot addr=0x11DD30
 * Purpose: floats*255+0.5 -> RGBA bytes
 * Info: proposed name ConvertVectorToRgba (T1); TU util
 * Notes: names_v2 v1 proposed LoadClutFromBrx__FP18CBinaryInputStreamP4CLUT (0.001)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
void func_0011DD30(VECTOR *pvec, int a, RGBA *prgba)
{
    prgba->r = (int)(pvec->x * 255.0f + 0.5f);
    prgba->g = (int)(pvec->y * 255.0f + 0.5f);
    prgba->b = (int)(pvec->z * 255.0f + 0.5f);
    prgba->a = a;
}
