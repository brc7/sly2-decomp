enum OID { OID_0 };
struct ALBRKS { char pad[0x1c]; unsigned int calbrks; OID aoidAlbrks[3]; };
struct ALARM { char pad[0x158]; ALBRKS *palbrks; };
/** @sly2-annot addr=0x13C448
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name AddBootyEntryOid (EXISTING); TU booty
 * Notes: RENAME proposed -> AddBootyEntryOid (existing_name_changes.csv)
 * Confidence: name high [EXISTING 0.80]; purpose not reviewed - evidence: arbitration; pre-existing symbol in
 *   symbol_addrs.txt
 * See: docs/engine/game-progression.md#5-booty-booty-tu-0x13bdf0,
 *   docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void AddAlarmAlbrk(ALARM *palarm, OID oid)
{
    ALBRKS *p = palarm->palbrks;
    if (p->calbrks < 3) p->aoidAlbrks[p->calbrks++] = oid;
}
