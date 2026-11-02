typedef struct CRef { int tagk; int tag; } CRef;
extern char D_002A79A0[], D_002A79A8[], D_002A79B8[];
void sprintf(char *pch, char *fmt, int n);
void RefGetSaveVarV(CRef *pref, char *a, char *b, char *c, int d);
void func_00129658(CRef *pref, CRef *prefSrc);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x1DAEA8
 * Purpose: Save var powerUps.mppukfAcquired[n] (has the power-up been bought)
 * Info: proposed name FPowerupAcquired (T1); TU screen
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
int FPowerupAcquired(int n) {
    CRef ref;
    char ach[0x20];
    CRef tmp;
    int r;
    if (n == -1) return 0;
    ref.tagk = -1;
    sprintf(ach, D_002A79A0, n);
    RefGetSaveVarV(&tmp, D_002A79A8, D_002A79B8, ach, 0);
    func_00129658(&ref, &tmp);
    Decref__4CRef(&tmp);
    r = ref.tag;
    Decref__4CRef(&ref);
    return r;
}
