struct S { char pad[0xB0]; int f; };
void InitLo(struct S *p);
/** @sly2-annot addr=0x211D08
 * Purpose: CID 0x7F Init (slot7): InitLo, +0xB0 = 1
 * Info: proposed name InitWidget127 (T2); class WIDGET127 slot7; TU hud
 * Notes: names_v2 v1 proposed PostBlotLoad__FP4BLOT (0.001) | v2: demoted to T2 (placeholder token in name:
 *   127)
 * Confidence: low (draft) [T2 0.60] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00211D08(struct S *p)
{
    InitLo(p);
    p->f = 1;
}
