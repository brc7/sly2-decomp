extern float D_002DDF0C;
extern void *D_002DD314;
extern int D_003D3A3C;
int func_00234A40(int);

static inline float *Receiver(float *p)
{
    register float *source __asm__("$4") = p;
    register float *copy __asm__("$5");
    __asm__("daddu %0,%1,$zero" : "=r"(copy) : "r"(source));
    return copy;
}

/** @sly2-annot addr=0x1C9058
 * Purpose: Store start time (g_clock real time D_002DDF0C) into caller's record, publish it (D_002DD314) and
 *   signal the loading-thread start sema
 * Info: proposed name BeginLoadingScreen (T1); TU prog
 * Notes: runtime5b: D_002DD314 = &h of LoadSwFromBrx during load (S56)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources: r5_1AE130+runtime
 * See: docs/engine/hud-ui.md#3-fonts-language-letterbox-loading-screen,
 *   docs/engine/world-sw.md#2-loading-a-level, docs/reference/functions/hud-ui.md#prog-0x1c8cc0-0x1c9940
 * @end-sly2-annot */
void func_001C9058(float *input)
{
    float now = D_002DDF0C;
    float *p = Receiver(input);
    int sema = D_003D3A3C;
    *p = now;
    D_002DD314 = p;
    func_00234A40(sema);
}
