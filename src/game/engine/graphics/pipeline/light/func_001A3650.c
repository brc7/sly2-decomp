struct K { int a, b; };
struct S { char pad[0x164]; struct K k; };
void RebuildLight(struct S *p);
/** @sly2-annot addr=0x1A3650
 * Purpose: Copy an 8-byte LM falloff and rebuild
 * Info: proposed name SetLightFallOff (T2); TU light
 * Notes: names_v2 v1 proposed OnLightRemove__FP5LIGHT (0.003)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3650(struct S *p, struct K *pk) { p->k = *pk; RebuildLight(p); }
