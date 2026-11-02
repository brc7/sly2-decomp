static inline float GMin(float a, float b) { return b < a ? b : a; }
static inline float GMax(float a, float b) { return a < b ? b : a; }
/** @sly2-annot addr=0x218790
 * Purpose: combine parent/child additive and multiplicative RGBA
 * Info: proposed name CombineSpriteColors (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void CombineSpriteColors(unsigned char *pa, unsigned char *pb, unsigned char *pc, unsigned char *pd,
                   unsigned char *pout0, unsigned char *pout1)
{
    int i;
    for (i = 0; i < 4; i++) {
        float x = GMax(GMin((float)(pa[i] - 127) * 0.007874016f, 1.0f), -1.0f);
        float y = GMax(GMin((float)(pc[i] - 127) * 0.007874016f, 1.0f), -1.0f);
        float e = (float)pb[i] - 127.0f;
        float d = (float)pd[i] - 127.0f;
        pout0[i] = (int)(GMax(GMin(x * y, 1.0f), -1.0f) * 127.0f + 128.0f);
        pout1[i] = (int)(GMax(GMin(x * d + e, 128.0f), -127.0f) + 127.0f);
    }
}
