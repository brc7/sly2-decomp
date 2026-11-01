struct RGBA {
    unsigned char r, g, b, a;
    RGBA() {}
    RGBA(int r_, int g_, int b_, int a_) { r = r_; g = g_; b = b_; a = a_; }
};
/** @sly2-annot addr=0x2275B8
 * Purpose: kind -> RGBA (0x35,0x36,0x37,0x3C,0x3F distinct colours, default grey), alpha 0x7F
 * Info: proposed name RgbaFromSpeakerCid (T1); TU tu_2275B8
 * Notes: verifier CORRECT: RgbaFromSpeakerKind? -> RgbaFromSpeakerCid
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/rendering.md#12-source-files, docs/engine/sound.md#5-source-files,
 *   docs/engine/sound.md#6-key-functions
 * @end-sly2-annot */
extern "C" RGBA func_002275B8(int k)
{
    RGBA clr;
    switch (k) {
    case 0x35: clr = RGBA(0x33, 0x66, 0x99, 0x7F); break;
    case 0x37: clr = RGBA(0x33, 0x99, 0x33, 0x7F); break;
    case 0x36: clr = RGBA(0x99, 0x33, 0x66, 0x7F); break;
    case 0x3F: clr = RGBA(0x99, 0x33, 0x33, 0x7F); break;
    case 0x3C: clr = RGBA(0x99, 0x33, 0x99, 0x7F); break;
    default: clr = RGBA(0x66, 0x66, 0x66, 0x7F); break;
    }
    return clr;
}
