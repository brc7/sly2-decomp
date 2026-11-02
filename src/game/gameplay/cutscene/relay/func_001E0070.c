struct SGG { char pad[0x84]; int calarm; void *apalarm[8]; };
void *memmove(void *d, void *s, int cb);
/** @sly2-annot addr=0x1E0070
 * Purpose: Remove an ALARM from the relay alarm list (memmove down)
 * Info: proposed name RemoveRelayAlarm (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E0070(struct SGG *psgg, void *palarm)
{
    int i;
    for (i = 0; i < psgg->calarm; i++) {
        if (psgg->apalarm[i] == palarm) {
            memmove(psgg->apalarm + i, psgg->apalarm + i + 1, (psgg->calarm - i) * 4 - 4);
            psgg->calarm--;
            return;
        }
    }
}
