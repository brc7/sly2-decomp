struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
struct G { char pad[0x4A4]; int n4A4; };
extern struct G D_002DDF40;
int func_00210AE0(void);
void func_00161868(void *p);
int func_0015E9C8(void *p, int a, int b, int c, int d, int e);
void func_001618B0(void *p);
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
/** @sly2-annot addr=0x140338
 * Purpose: On a POCTL button press (mask 0x200, Select?): if 0x210AE0 allows, calls 0x161868/0x1618B0 on g_sw
 *   (find class 0x605 obj), plays sound 0x32, stamps g_sw+0x4754 = g_clock.t and bumps g_cm+0x4A4
 * Info: proposed name AcknowledgeStepplayerHint (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00140338(void)
{
    if (func_00210AE0()) {
        func_00161868(D_003DD490);
        if (func_0015E9C8(D_003DD490, 0x605, 0x5F, 0, 0, 0)) func_001618B0(D_003DD490);
        func_001FB2F8(func_001FBB90(0x32), 0);
        *(float *)(D_003DD490 + 0x4754) = D_002DDED0.t;
        D_002DDF40.n4A4++;
    }
}
