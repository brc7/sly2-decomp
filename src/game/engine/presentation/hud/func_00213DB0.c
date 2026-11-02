void func_00213928(void *p, int k);
void func_00213B00(void *p, int k);
/** @sly2-annot addr=0x213DB0
 * Purpose: stop old *pid, play new id, store
 * Info: proposed name SwitchWidgetAnim (T1); TU hud
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00213DB0(void *p, int k, int *pk)
{
    if (k == *pk)
        return;
    if (*pk != -1)
        func_00213928(p, *pk);
    *pk = k;
    if (k != -1)
        func_00213B00(p, k);
}
