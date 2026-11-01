struct G;
extern struct G D_003DD490;
extern char D_002DDED0[], D_002DEB30[];
struct S { char pad[0x24]; int x24; };
void func_001D9910(struct S *p);
void func_0014D1D0(void *p, int n, float g);
void func_0020F030(struct G *p, int n);
void func_001619A0(void *p, int n, int m);
/** @sly2-annot addr=0x1AEA40
 * Purpose: Screen base hide (0x1D9910), clock rate 1, DecrementSwHandsOff, BroadcastGameEvent 0x2C
 * Info: proposed name HidePausemenu (T1); class PAUSEMENU; TU menu
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001AEA40(struct S *p)
{
    func_001D9910(p);
    p->x24 = 0;
    func_0014D1D0(D_002DDED0, 0, 1.0f);
    func_0020F030(&D_003DD490, 0);
    func_001619A0(D_002DEB30, 0x2C, 0);
}
