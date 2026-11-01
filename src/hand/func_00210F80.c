extern float D_003E1BFC;
extern struct CLOCK { float t; float dt; } D_002DDED0;
extern char D_002A8AB0[];
void fiprintf(void *f, const char *fmt, ...);
/** @sly2-annot addr=0x210F80
 * Purpose: STRINGGEN id 0x2D1: countdown "%d:%02d" of max(0, SW+0x476C - g_clock.t)
 * Info: proposed name BuildTimerText (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00210F80(void *f)
{
    float t = -D_002DDED0.t + D_003E1BFC; int n; t = (t < 0.0f) ? 0.0f : t;
    n = (int)t;
    {
        int q = n / 60;
        fiprintf(f, D_002A8AB0, q, n - q * 60);
    }
}
