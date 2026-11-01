struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern int D_002E1E40;
extern char D_002DEB30[];
struct VE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VE *vt; char pad1[0x38 - 0x10]; int x38; char pad2[4]; int x40; };
struct S { char pad[0x10]; int x10; char pad1[4]; int x18; char pad2[0x30 - 0x1C]; int x30; float f34; int x38; };
void func_00171C18(int a, int b);
void PopBhv(void *p);
struct O *func_00205438(int a, int b);
void func_00171258(struct O *o, int a, int b, int c);
void BroadcastGameEvent(void *p, int a, int b);
#define VCALL0(o, n) ((void (*)())(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d)
/** @sly2-annot addr=0x1BA828
 * Purpose: BHV174 state: 0 resume script (0xA9), 1 chase player (0x9E target=g_pjt), 2 behaviour 0x9D +
 *   BroadcastGameEvent 0x33
 * Info: proposed name SetBhv174State (T2); TU tu_1B9F80
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1b9f80-0x1b9f80-0x1bab50
 * @end-sly2-annot */
void func_001BA828(struct S *p, int st)
{
    if (st == p->x30) return;
    if (p->x30 == 0) func_00171C18(p->x18, p->x10 + 0x1090);
    PopBhv(&p->x18);
    p->x30 = st;
    p->f34 = D_002DDED0.t;
    switch (st) {
    case 0: {
        struct O *o = func_00205438(p->x10, 0xA9);
        func_00171258(o, p->x38, 0, p->x10 + 0x1090);
        VCALL0(o, 8);
        break;
    }
    case 1: {
        struct O *o = func_00205438(p->x10, 0x9E);
        o->x38 = D_002E1E40;
        VCALL0(o, 8);
        break;
    }
    case 2: {
        struct O *o = func_00205438(p->x10, 0x9D);
        o->x40 = p->x10 + 0x1060;
        VCALL0(o, 8);
        BroadcastGameEvent(D_002DEB30, 0x33, p->x10);
        break;
    }
    }
}
