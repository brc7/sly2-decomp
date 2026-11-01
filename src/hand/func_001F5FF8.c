struct E { int oid; char pad[0x24]; };
struct S { char pad[0xB8]; int c; int pad2; struct E *ae; };
/** @sly2-annot addr=0x1F5FF8
 * Purpose: Find glob (globset +0xB8 array, 0x28 stride) by oid
 * Info: proposed name PglobFindSo (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct E *func_001F5FF8(struct S *p, int oid)
{
    int i;
    for (i = 0; i < p->c; i++) { struct E *pe = &p->ae[i]; if (pe->oid == oid) return pe; }
    return 0;
}
