int func_00132B10(void *p, int n);
/** @sly2-annot addr=0x184670
 * Purpose: Return the GRABSPOT sub-object of an object by class (CID88 +0x150, CID27 +0x130, CID34 +0xE0,
 *   CID12 +0x190, ...) or NULL
 * Info: proposed name PgrabspotFromLo (T1); TU grabspot
 * Notes: names_v2 C proposed PdartOldestHbskStuck__FP4HBSK (0.638)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void *func_00184670(char *p)
{
    if (!p) return 0;
    if (func_00132B10(p, 0x58)) return p + 0x150;
    if (func_00132B10(p, 0x1B)) return p + 0x130;
    if (func_00132B10(p, 0x22)) return p + 0xE0;
    if (func_00132B10(p, 0xC)) return p + 0x190;
    if (func_00132B10(p, 0x23)) return p + 0xE0;
    if (func_00132B10(p, 0x13)) return p + 0x100;
    if (func_00132B10(p, 0xB)) return p + 0x190;
    if (func_00132B10(p, 0xD)) return p + 0x190;
    if (func_00132B10(p, 0x27)) return p + 0xE0;
    return 0;
}
