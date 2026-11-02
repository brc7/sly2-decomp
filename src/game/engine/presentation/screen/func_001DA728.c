extern char D_002D85E8[];
struct S { int a, b, c; void *vt; char pad[0x10]; int n20; };
void func_001DA7D0(struct S *p);
/** @sly2-annot addr=0x1DA728
 * Purpose: Base screen constructor: vtable 0x2D85E8, widget oid(+0x20)=-1, reset
 * Info: proposed name SCREEN_ctor (T2); class SCREEN; TU screen
 * Notes: agent qualifier: (ctor)
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
struct S *func_001DA728(struct S *p) {
    p->a = 0;
    p->b = 0;
    p->vt = D_002D85E8;
    p->n20 = -1;
    func_001DA7D0(p);
    return p;
}
