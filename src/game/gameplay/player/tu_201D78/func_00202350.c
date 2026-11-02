struct S { char pad[0x10]; long fl; char pad2[0x58]; int n; };
/** @sly2-annot addr=0x202350
 * Purpose: filter callback for IntersectSwBoundingBox
 * Info: proposed name FFilterReconOccluder (T2); TU tu_201D78
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
int func_00202350(int a, struct S *p) { int r = 0; if (!(p->fl & 0x1000000000000L)) r = p->n > 0; return r; }
