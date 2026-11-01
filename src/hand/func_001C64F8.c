struct S { char pad[0x1A4]; int a; int x; int b; int y; int c; };
/** @sly2-annot addr=0x1C64F8
 * Purpose: 16-byte helper
 * Info: proposed name Dprize_Stub64F8 (T2); TU prize
 * Confidence: low (draft) [T2 0.15] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C64F8(struct S *p, int a, int b, int c) { p->a = a; p->b = b; p->c = c; }
