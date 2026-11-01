typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[9]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x28 - 0x10]; ulong fl; char pad3[0x12C - 0x30]; float g12C; };
void func_001F7D90(struct P *, float);
void func_001F7D10(struct P *, float);
/** @sly2-annot addr=0x1F7C68
 * Purpose: dur<=0: rate 0, alpha 1; else re-Add if removed (alpha 0) and rate 1/dur
 * Info: proposed name FadeSoIn (T1); TU so
 * Notes: names_v2 v1 proposed PresetSoAccel__FP2SOf (0.029)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001F7C68(struct P *p, float f)
{
    if (f <= 0.0f) {
        func_001F7D90(p, 0.0f);
        func_001F7D10(p, 1.0f);
    } else {
        if (p->fl & (1L << 56)) {
            p->pvt->a[8].pfn((char *)p + p->pvt->a[8].d);
            p->g12C = 0.0f;
        }
        func_001F7D90(p, 1.0f / f);
    }
}
