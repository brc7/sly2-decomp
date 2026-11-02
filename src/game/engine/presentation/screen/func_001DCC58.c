extern char D_002D8468[];
struct S { int a, b, c; void *vt; char pad[0x10]; int n20; };
struct S *func_001DA728(struct S *p);
void ClearJobList(struct S *p);
/** @sly2-annot addr=0x1DCC58
 * Purpose: Screen subclass with vtable 0x2D8468, widget oid 0x4E0
 * Info: proposed name SCREEN4E0_ctor (T2); class SCREEN4E0; TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
struct S *func_001DCC58(struct S *p) {
    func_001DA728(p);
    p->vt = D_002D8468;
    p->n20 = 0x4E0;
    ClearJobList(p);
    return p;
}
