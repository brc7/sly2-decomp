struct ALARM;
struct SGG { char pad[0x84]; int calarm; ALARM *apalarm[8]; };
/** @sly2-annot addr=0x1E0020
 * Purpose: Add an ALARM to the relay's alarm list (+0x84 calarm, +0x88 apalarm[8]) if absent and room
 * Info: proposed name EnsureRelayAlarm (EXISTING); TU relay
 * Notes: agent/verifier proposes EnsureRelayAlarm for this existing name (see existing_name_changes.csv) |
 *   RENAME proposed -> EnsureRelayAlarm (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.75] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r6b_rest+arbitration
 * See: docs/engine/cutscenes.md#1-relay-and-jsg-the-sequence-engine-tu-0x1dfad8-0x1e5d80-file-name-unknown-lilly-jsgc,
 *   docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void EnsureSggAlarm(SGG *psgg, ALARM *palarm)
{
    int i;
    for (i = 0; i < psgg->calarm; i++) {
        if (psgg->apalarm[i] == palarm) return;
    }
    if ((unsigned int)psgg->calarm < 8) psgg->apalarm[psgg->calarm++] = palarm;
}
