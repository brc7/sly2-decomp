struct RGBA { unsigned char r, g, b, a; };
struct VECTOR { float x, y, z, w; } __attribute__((aligned(16)));
extern int D_003E12C8;
void func_0011DCD8(struct RGBA *prgba, struct VECTOR *pv);
void func_001ED2D8(struct VECTOR *pv);
void ConvertVectorToRgba(struct VECTOR *pv, int a, struct RGBA *prgba);
/** @sly2-annot addr=0x1ED278
 * Purpose: If color correction enabled (D_003E12C8): RGBA -> vector -> ColorCorrectVector -> RGBA
 * Info: proposed name ColorCorrectRgba (T1); TU shd
 * Notes: names_v2 C proposed PropagateSur__FP3SUR (0.638) | textures format doc cites this function by this
 *   name (CONFIRMED(code) in the doc)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r6b_rest+format-doc:textures
 * See: docs/engine/camera.md#5-key-functions,
 *   docs/formats/textures.md#2-clut-record-0x10-byte-header-cstatic,
 *   docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_001ED278(struct RGBA *prgba)
{
    struct VECTOR v;
    struct RGBA rgba;
    if (D_003E12C8) {
        rgba = *prgba;
        func_0011DCD8(&rgba, &v);
        func_001ED2D8(&v);
        ConvertVectorToRgba(&v, prgba->a, prgba);
    }
}
