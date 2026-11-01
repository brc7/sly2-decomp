struct P { char pad[0x1BC]; float g; };
struct S;
extern struct S D_002BBB60;
float func_0011D948(struct S *, float);
void func_002321E8(int, int);
/** @sly2-annot addr=0x1FE9F8
 * Purpose: Clamp and send master volume (snd_SetMasterVolume(0x10,..))
 * Info: proposed name SetSndmgrMasterVolume (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FE9F8(struct P *p, float x)
{
    float g = func_0011D948(&D_002BBB60, x);
    int n = (int)(g * 1024.0f);
    if ((int)(p->g * 1024.0f) != n) {
        p->g = g;
        func_002321E8(0x10, n);
    }
}
