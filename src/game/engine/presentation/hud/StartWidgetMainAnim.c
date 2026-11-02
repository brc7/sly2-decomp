struct VE { short d; short pad; void (*pf)(void *, int); };
struct S { char pad[0xC]; struct VE *vt; char pad2[0x6C]; int x7c; char pad3[4]; void *x84; char pad4[0x24]; int xac; };
void func_00217BA0(void *p);
void func_00214300(struct S *p, int a);
/** @sly2-annot addr=0x213830
 * Purpose: start anim id on main sprite (+0x84), slot21(1), join exclusive list if flag, +0x7C = id
 * Info: proposed name StartWidgetMainAnim (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void StartWidgetMainAnim(struct S *p, int a)
{
    func_00217BA0(p->x84);
    p->vt[21].pf((char *)p + p->vt[21].d, 1);
    if (p->xac)
        func_00214300(p, 0);
    p->x7c = a;
}
