struct P { char pad[0x10]; float g; };
struct S;
extern struct S D_002BBB60;
float func_0011D948(struct S *, float);
void func_001FB658(struct P *, int, int);
/** @sly2-annot addr=0x1FB1F8
 * Purpose: Clamp 0..1 (GLimitLm g_lm01) and set vol, dirty 0x10 if changed at 1/1024 resolution
 * Info: proposed name SetSndVolume (T1); TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FB1F8(struct P *p, float x)
{
    float g = func_0011D948(&D_002BBB60, x);
    if ((int)(g * 1024.0f) != (int)(p->g * 1024.0f)) {
        p->g = g;
        func_001FB658(p, 0x10, 0x10);
    }
}
