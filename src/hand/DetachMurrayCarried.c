struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
struct VE { short d; short pad; void (*pfn)(); };
struct X { char pad[0x58]; int x58; int x5C; };
struct O { char pad[0xC]; struct VE *vt; char pad1[0x180 - 0x10]; struct X *x180; };
struct P { char pad[0x13F0]; struct O *x13F0; int a13F4[4]; int x1404; int x1408; char pad2[0x1430 - 0x140C]; struct X *x1430; float f1434; char pad3[0x1600 - 0x1438]; int a1600[4]; int x1610, x1614; int a1618[4]; };
void ReleaseAct(int *p);
void func_002303A0(struct X *x, int a, int b);
void func_001F6860(struct X *x, int a);
void SetLoParent_1A58D0(struct X *x, int a, int b);
void RecalcSwOxfFilterForObject(void *g, struct P *p);
/** @sly2-annot addr=0x1B7D20
 * Purpose: Free the 12 grip constraints (0x12E460), notify object (vt3 msg 6), restore its XF roles,
 *   SetSoTreeFlag2(0), SetLoParent(0,0xF); remember it as recently released (x1430, time)
 * Info: proposed name DetachMurrayCarried (T1); TU murray
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void DetachMurrayCarried(struct P *p)
{
    struct O *o;
    struct X *x;
    ReleaseAct(&p->a13F4[0]);
    ReleaseAct(&p->a13F4[1]);
    ReleaseAct(&p->a13F4[2]);
    ReleaseAct(&p->a13F4[3]);
    ReleaseAct(&p->a1600[0]);
    ReleaseAct(&p->a1600[1]);
    ReleaseAct(&p->a1600[2]);
    ReleaseAct(&p->a1600[3]);
    ReleaseAct(&p->a1618[0]);
    ReleaseAct(&p->a1618[1]);
    ReleaseAct(&p->a1618[2]);
    ReleaseAct(&p->a1618[3]);
    o = p->x13F0;
    if (!o) return;
    ((void (*)())o->vt[3].pfn)((char *)o + o->vt[3].d, 6, 0, 0, 0, 0);
    x = p->x13F0->x180;
    func_002303A0(x, x->x58, p->x1404);
    func_002303A0(x, x->x5C, p->x1408);
    func_001F6860(x, 0);
    SetLoParent_1A58D0(x, 0, 0xF);
    p->x1430 = x;
    p->f1434 = D_002DDED0.t;
    RecalcSwOxfFilterForObject(D_003DD490, p);
}
