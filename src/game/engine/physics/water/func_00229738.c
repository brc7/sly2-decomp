struct N { int a[2]; struct N *next; };
struct S { char pad[0x138]; struct N *first; };
void func_001F5078(struct S *p, struct N *n);
/** @sly2-annot addr=0x229738
 * Purpose: unlink XA from SO list then 0x1F5078
 * Info: proposed name RemoveSoXa (T1); TU water
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verify_side
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
void func_00229738(struct S *p, struct N *n)
{
    struct N **pp;
    for (pp = &p->first; *pp; pp = &(*pp)->next) {
        if (*pp == n) { *pp = n->next; n->next = 0; break; }
    }
    func_001F5078(p, n);
}
