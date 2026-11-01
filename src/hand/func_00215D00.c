struct RGBA { unsigned char r, g, b, a; };
struct S { char pad[0x14]; struct RGBA c1; struct RGBA c0; };
void func_002160D8(struct S *p);
/** @sly2-annot addr=0x215D00
 * Purpose: set base and current colour
 * Info: proposed name CRichText2_SetBaseColor (T2); class CRichText; TU hud
 * Notes: names_v2 C proposed HideBlot__FP4BLOT (0.167) | COLLISION: CRichText_SetBaseColor already used at
 *   0x15F9A8 (existing symbol); renamed CRichText_SetBaseColor_215D00 | v3: superseded
 *   CRichText_SetBaseColor_215D00 | top50 finding 8: hud-TU rich text (colours +0x14/+0x18) is a second
 *   class; Reset__9CRichText 0x15F970 keeps CRichText. CRichText2 is a placeholder class word (T2); resolves
 *   the v2 T3 collision | T2: placeholder token CRichText2
 * Confidence: low (draft) [T2 0.85] - evidence: semantic review; static naming pass; sources:
 *   r7c_hud+verdicts_followups+top50_round(CRichText2 class split)
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00215D00(struct S *p, struct RGBA *pc)
{
    p->c0 = *pc;
    p->c1 = p->c0;
    func_002160D8(p);
}
