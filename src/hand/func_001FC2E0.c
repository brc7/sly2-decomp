struct P { char pad[0x70]; float a[6]; char pad2[0x3BC - 0x88]; int fl; };
struct S;
extern struct S D_002BBB60;
float func_0011D948(struct S *, float);
void func_001FB658(struct P *, int, int);
/** @sly2-annot addr=0x1FC2E0
 * Purpose: Set volume of sub-group k<6 (clamped), dirty 0x80
 * Info: proposed name SetStreamsndGroupVolume (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC2E0(struct P *p, unsigned int k, float g)
{
    if (k < 6) {
        float f = func_0011D948(&D_002BBB60, g);
        if ((int)(f * 1024.0f) != (int)(p->a[k] * 1024.0f)) {
            p->fl |= 1 << k;
            p->a[k] = f;
            func_001FB658(p, 0x80, 0x80);
        }
    }
}
