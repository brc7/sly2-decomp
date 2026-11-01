struct G;
extern struct G D_003DD490;
extern char D_002DDED0[], D_002DEB30[];
void func_001D98B0(void);
void func_0014D1D0(void *p, int n, float g);
void IncrementSwHandsOff(struct G *p, int n);
void BroadcastGameEvent(void *p, int n, int m);
/** @sly2-annot addr=0x1AE9E8
 * Purpose: Screen base show (0x1D98B0), clock rate 0 (pause), IncrementSwHandsOff, BroadcastGameEvent 0x28
 * Info: proposed name ShowPausemenu (T1); class PAUSEMENU; TU menu
 * Notes: runtime5b: clock slot0 0 + hands-off 1 in pause (S54)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources: r5b_rest+runtime
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001AE9E8(void)
{
    func_001D98B0();
    func_0014D1D0(D_002DDED0, 0, 0.0f);
    IncrementSwHandsOff(&D_003DD490, 0);
    BroadcastGameEvent(D_002DEB30, 0x28, 0);
}
