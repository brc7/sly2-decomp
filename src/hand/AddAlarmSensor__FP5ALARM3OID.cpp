enum OID { OID_0 };
struct SENSORS { char pad[0x1c]; unsigned int coidSensors; OID aoidSensors[3]; };
struct ALARM { char pad[0x244]; SENSORS *psensors; };
/** @sly2-annot addr=0x1C7A10
 * Purpose: Append OID to the list behind this+0x244 (count +0x1C, max 3)
 * Info: proposed name AddLootEntryOid (EXISTING); TU prize
 * Notes: agent/verifier proposes AddLootObject for this existing name (see existing_name_changes.csv) |
 *   DISPUTED (see disputed.csv) | RENAME proposed -> AddLootEntryOid (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.85] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r5_1AE130+arbitration
 * See: docs/engine/game-progression.md#4-collectibles-and-loot-prizecpp-0x1c4e50,
 *   docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void AddAlarmSensor(ALARM *palarm, OID oid)
{
    SENSORS *p = palarm->psensors;
    if (p->coidSensors < 3) p->aoidSensors[p->coidSensors++] = oid;
}
