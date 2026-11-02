int func_00127DB8(void *a, void *b, void *c, void *d);
void func_00129658(void *a, int b);
/** @sly2-annot addr=0x127D78
 * Purpose: PrefFindBinding wrapper: copies binding into *pref, returns found
 * Info: proposed name CFrame_FFindBinding (T1); class CFrame; TU frame
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r1_116918+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18,
 *   docs/reference/functions/splice-scripting.md#frame-0x127b18-0x128138
 * @end-sly2-annot */
int FFindBinding__6CFrameUiiP4CRef(void *a, void *b, void *c, void *d)
{
    int v = func_00127DB8(a, b, c, d);
    if (v) {
        if (d)
            func_00129658(d, v);
        return 1;
    }
    return 0;
}
