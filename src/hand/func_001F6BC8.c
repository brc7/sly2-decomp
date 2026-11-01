struct E { int a[3]; int n; int b[2]; int x18; char pad[0x94]; };
struct S { char pad[0xE0]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F6BC8
 * Purpose: Set SOPART +0x18 (grfzpk) for parts with id n
 * Info: proposed name SetSoPartsField18ById (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F6BC8(struct S *p, int n, int x) { int i; for (i = 0; i < p->c; i++) { struct E *pe = &p->ae[i]; if (pe->n == n) pe->x18 = x; } }
