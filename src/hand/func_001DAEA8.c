typedef struct CRef { int tagk; int tag; } CRef;
extern char D_002A79A0[], D_002A79A8[], D_002A79B8[];
void func_0021ECD8(char *pch, char *fmt, int n);
void func_001D8A38(CRef *pref, char *a, char *b, char *c, int d);
void func_00129658(CRef *pref, CRef *prefSrc);
void func_00129EC8(CRef *pref);
/** @sly2-annot addr=0x1DAEA8
 * Purpose: Save var powerUps.mppukfAcquired[n] (has the power-up been bought)
 * Info: proposed name FPowerupAcquired (T1); TU screen
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
int func_001DAEA8(int n) {
    CRef ref;
    char ach[0x20];
    CRef tmp;
    int r;
    if (n == -1) return 0;
    ref.tagk = -1;
    func_0021ECD8(ach, D_002A79A0, n);
    func_001D8A38(&tmp, D_002A79A8, D_002A79B8, ach, 0);
    func_00129658(&ref, &tmp);
    func_00129EC8(&tmp);
    r = ref.tag;
    func_00129EC8(&ref);
    return r;
}
