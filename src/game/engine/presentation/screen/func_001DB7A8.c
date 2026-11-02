extern char D_002D84E8[];
struct S { int a, b, c; void *vt; char pad[0x10]; int n20; };
struct S *func_001DA728(struct S *p);
void ClearThiefnet(struct S *p);
/** @sly2-annot addr=0x1DB7A8
 * Purpose: Screen subclass with vtable 0x2D84E8, widget oid 0x378, 64-entry selection table
 * Info: proposed name SCREEN378_ctor (T2); class SCREEN378; TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
struct S *func_001DB7A8(struct S *p) {
    func_001DA728(p);
    p->vt = D_002D84E8;
    p->n20 = 0x378;
    ClearThiefnet(p);
    return p;
}
