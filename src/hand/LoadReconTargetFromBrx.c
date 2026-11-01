struct S { char pad[0xE0]; float gE0; float gE4; float gE8; char p2[8]; int nF4; int nF8; char p3[8]; float g104; };
void LoadXfmFromBrx(struct S *p);
/** @sly2-annot addr=0x2035A8
 * Purpose: CID 0x2E LoadFromBrx: defaults 75 / 0.25 / 5000, copy defaults
 * Info: proposed name LoadReconTargetFromBrx (T1); class CID46 slot14; TU tu_201D78
 * Notes: level_parser CONFIRMED-code (layout read from the matched function; class absent from samples) |
 *   whole_game: loader read in all 43 levels (base reader only, nothing extra; CONFIRMED code+parse)
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7c_hud+parse-code+parse-verified
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void LoadReconTargetFromBrx(struct S *p)
{
    p->gE4 = 0.0f;
    p->gE0 = 75.0f;
    p->gE8 = 0.25f;
    p->g104 = 5000.0f;
    LoadXfmFromBrx(p);
    if (p->gE4 == 0.0f) p->gE4 = p->gE0;
    if (p->nF8 == -2) p->nF8 = p->nF4;
}
