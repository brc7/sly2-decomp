struct RGBA { unsigned char r, g, b, a;
  RGBA() {}
  RGBA(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_) { r = r_; g = g_; b = b_; a = a_; }
};
struct BF { unsigned short r : 5; unsigned short g : 5; unsigned short b : 5; unsigned short a : 1; };
union U { unsigned short w; BF bf; };
extern "C" { extern int D_003E12C8; void ColorCorrectRgba(RGBA *prgba); void func_001ED1A8(U *pu); }
static inline U Pack(RGBA c) { U u; u.bf.r = c.r >> 3; u.bf.g = c.g >> 3; u.bf.b = c.b >> 3; u.bf.a = c.a >> 7; return u; }
/** @sly2-annot addr=0x1ED1A8
 * Purpose: Color-correct one 5551 pixel (expand, ColorCorrectRgba, repack) when correction is on (D_003E12C8)
 * Info: proposed name ColorCorrectRgba16 (T2); TU shd
 * Notes: names_v2 B proposed PqwGifsBitmapUpload__Fi (0.889)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6b_rest
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148,
 *   docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void func_001ED1A8(U *pu)
{
    if (!D_003E12C8) return;
    RGBA c(pu->bf.r << 3, pu->bf.g << 3, pu->bf.b << 3, pu->bf.a ? 0xFF : 0);
    ColorCorrectRgba(&c);
    *pu = Pack(c);
}
