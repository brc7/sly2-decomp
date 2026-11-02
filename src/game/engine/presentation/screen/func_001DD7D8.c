extern char D_002D8368[];
struct S { int a, b, c; void *vt; char pad[0x10]; int n20; };
struct S *func_001DA728(struct S *p);
void func_001DA7D0(struct S *p);
/** @sly2-annot addr=0x1DD7D8
 * Purpose: Screen subclass with vtable 0x2D8368, widget oid 0x5D2
 * Info: proposed name SCREEN5D2_ctor (T2); class SCREEN5D2; TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
struct S *func_001DD7D8(struct S *p) {
    func_001DA728(p);
    p->vt = D_002D8368;
    p->n20 = 0x5D2;
    func_001DA7D0(p);
    return p;
}
