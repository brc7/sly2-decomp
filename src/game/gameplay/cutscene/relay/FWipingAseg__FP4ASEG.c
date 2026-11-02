struct ASEG { char pad[0x84]; int c; int a[1]; };
/** @sly2-annot addr=0x1E00E8
 * Purpose: Return whether the alarm list (+0x84/+0x88) contains x
 * Info: proposed name FRelayHasAlarm (EXISTING); TU relay
 * Notes: agent/verifier proposes FRelayHasAlarm for this existing name (see existing_name_changes.csv) |
 *   RENAME proposed -> FRelayHasAlarm (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.75] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r6b_rest+arbitration
 * See: docs/engine/cutscenes.md#1-relay-and-jsg-the-sequence-engine-tu-0x1dfad8-0x1e5d80-file-name-unknown-lilly-jsgc,
 *   docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
int FWipingAseg__FP4ASEG(struct ASEG *paseg, int n)
{
    int i;
    for (i = 0; i < paseg->c; i++) {
        if (paseg->a[i] == n) return 1;
    }
    return 0;
}
