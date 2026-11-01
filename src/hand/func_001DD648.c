extern char D_002D83E8[];
struct S { int a, b, c; void *vt; char pad[0x10]; int n20; };
struct S *func_001DA728(struct S *p);
void func_001DA7D0(struct S *p);
/** @sly2-annot addr=0x1DD648
 * Purpose: Screen subclass with vtable 0x2D83E8, widget oid 0x52D
 * Info: proposed name SCREEN52D_ctor (T2); class SCREEN52D; TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
struct S *func_001DD648(struct S *p) {
    func_001DA728(p);
    p->vt = D_002D83E8;
    p->n20 = 0x52D;
    func_001DA7D0(p);
    return p;
}
