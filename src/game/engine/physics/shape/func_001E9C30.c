
struct MSG { int cb; int nKind; };
struct S { char pad[0x180]; void *pv; };
void *func_001AE2A0(struct MSG *pmsg);
void func_001E9C88(void *pv, struct S *p);
/** @sly2-annot addr=0x1E9C30
 * Purpose: Allocate (0x9C) and build the shape's volume/segment table once (+0x180)
 * Info: proposed name EnsureShapeVolumeSegments (T2); TU shape
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58,
 *   docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E9C30(struct S *p)
{
    struct MSG msg;
    if (!p->pv) {
        msg.cb = 0x9C;
        msg.nKind = 0x10;
        p->pv = func_001AE2A0(&msg);
        func_001E9C88(p->pv, p);
    }
}
