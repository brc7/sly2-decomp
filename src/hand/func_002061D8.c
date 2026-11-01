typedef int qword __attribute__((mode(TI)));
struct VE { short d; short pad; void (*pf)(void *); };
struct O { char pad[0xC]; struct VE *vt; char pad2[0x50 - 0x10]; int n50; };
struct C3 { char pad[0x24]; int n24; };
struct K { int a; int b; };
struct S {
    char pad0[0x18]; int n18; char pad1[0x3C - 0x1C]; struct C3 *p3C; char pad2[0x58 - 0x40]; int n58; int n5C;
    char pad3[0x260 - 0x60]; float f260; char pad4[0x2EC - 0x264]; struct O *p2EC; int n2F0;
    char pad5[0x3F4 - 0x2F4]; struct O *p3F4; struct O *p3F8; char pad6[0x498 - 0x3FC]; void *p498; void *p49C;
    char pad7[0xFC4 - 0x4A0]; void *pFC4;
};
extern char D_002C3CF8[];
extern char D_003DD490[];
extern struct K D_002A8608;
extern qword D_002A2DE0;
void func_00203A38(struct S *p);
void func_001A64E8(struct S *p, int n, void *pv);
void func_00204DA8(struct S *p, float f);
void func_002303A0(struct S *p, int n, int m);
void func_002080F0(struct S *p);
void func_0020AAA8(struct S *p);
void *func_001AE4F8(int cb);
void func_0020CE28(void *pv, struct S *p);
void func_0020D1B0(void *pv, struct S *p);
void func_00149A50(void *g, struct S *p, struct K k, int n);
struct O *func_0013E3C8(int a, int b, int c, int d);
void func_00170C90(struct O *po, int a, int b, float f);
void *func_001F4C58(struct S *p, int n, int m);
void *func_0012DC18(struct S *p, void *x, int a, int b, int c, qword q);

/** @sly2-annot addr=0x2061D8
 * Purpose: STEPGUARD vt15: PostStepLoad, bind oid table, SetStepGravityRatio(+0x260), attach children,
 *   resolve anim bindings (0x2080F0) and attack oids (0x20AAA8), create the two helper components
 *   +0x498/+0x49C (next TU 0x20CE28/0x20D1B0), create cid 0x71 object (oid 0x2CB), remove pickpocket glints
 *   from world, create scale act +0xFC4
 * Info: proposed name PostStepguardLoad (T1); class STEPGUARD slot15; TU stepguard
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_002061D8(struct S *p)
{
    void *x;
    func_00203A38(p);
    func_001A64E8(p, 1, D_002C3CF8);
    func_00204DA8(p, p->f260);
    func_002303A0(p, p->n58, 2);
    func_002303A0(p, p->n5C, 2);
    func_002080F0(p);
    func_0020AAA8(p);
    p->p498 = func_001AE4F8(0x10);
    func_0020CE28(p->p498, p);
    p->p49C = func_001AE4F8(0x14);
    func_0020D1B0(p->p49C, p);
    if (!p->p3C->n24)
        func_00149A50(D_003DD490, p, D_002A8608, 0);
    p->p2EC = func_0013E3C8(0x71, 0, 0x2CB, -1);
    func_00170C90(p->p2EC, -1, -1, 40.0f);
    p->n2F0 = p->p2EC->n50;
    p->p2EC->vt[9].pf((char *)p->p2EC + p->p2EC->vt[9].d);
    if (p->p3F4)
        p->p3F4->vt[9].pf((char *)p->p3F4 + p->p3F4->vt[9].d);
    if (p->p3F8)
        p->p3F8->vt[9].pf((char *)p->p3F8 + p->p3F8->vt[9].d);
    x = func_001F4C58(p, p->n18, 0xC);
    if (x)
        p->pFC4 = func_0012DC18(p, x, 0, 7, 2, D_002A2DE0);
}
