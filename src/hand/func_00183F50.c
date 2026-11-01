struct S { char pad[0x38]; float g; char p2[0xC0]; int nfc; };
extern float D_002BE900;
void func_001A5C48(struct S *p);
/** @sly2-annot addr=0x183F50
 * Purpose: vt15: PostLoLoad, mask default 7, default radius D_002BE900
 * Info: proposed name PostCid35Load (T2); class CID35 slot15; TU grabspot
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558,
 *   docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00183F50(struct S *p)
{
    func_001A5C48(p);
    if (!p->nfc) p->nfc = 7;
    if (p->g == 1e10f) p->g = D_002BE900;
}
