typedef int qword __attribute__((mode(TI)));
typedef struct { short d; short pad; void *pfn; } VTE;
struct O { char pad[0xC]; VTE *vt; char pad2[0x184 - 0x10]; int n184; int n188; };
struct S { struct O *po; int state; float t; struct O *pc; };
struct L { struct O *pc; int pad; struct O *po; int a; int b; int pad2[3]; qword v20; qword v30; char pad3[0x20]; };
extern struct { float t; float dt; } D_002DDED0;
void func_002333B4(void *p, int c, int n);
qword func_001A62E8(struct O *po);
void func_001F7598(struct L *pl);
/** @sly2-annot addr=0x20D3C0
 * Purpose: state 2 -> InflictZap with hazard +0x184/+0x188
 * Info: proposed name SetHazardWatchState (T2); TU tu_20CE28
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/guards-ai.md#tu_20ce28-0x20ce28-0x20d5d0
 * @end-sly2-annot */
void func_0020D3C0(struct S *p, int state)
{
    if (state != p->state) {
        p->state = state;
        p->t = D_002DDED0.t;
        if (state == 2) {
            struct L l;
            func_002333B4(&l, 0, sizeof(l));
            l.pc = p->pc;
            l.po = p->po;
            l.a = p->pc->n188;
            l.b = p->pc->n184;
            l.v20 = func_001A62E8(p->po);
            l.v30 = ((qword (*)(void *))p->po->vt[33].pfn)((char *)p->po + p->po->vt[33].d);
            func_001F7598(&l);
        }
    }
}
