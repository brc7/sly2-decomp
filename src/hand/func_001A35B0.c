struct S { char pad[0xE0]; int xe0; };
void func_001A3B38(struct S *p);
void func_001A3968(struct S *p);
/** @sly2-annot addr=0x1A35B0
 * Purpose: Invalidate, set kind (xE0), RebuildLight
 * Info: proposed name SetLightKind (T1); TU light
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A35B0(struct S *p, int a) { func_001A3B38(p); p->xe0 = a; func_001A3968(p); }
