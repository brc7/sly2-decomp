typedef int qword __attribute__((mode(TI)));
struct VE { short d; short pad; void (*pfn)(); };
struct X { char pad[0xC]; struct VE *vt; };
struct O { char pad[0xC]; struct VE *vt; char pad1[0x180 - 0x10]; struct X *x180; };
struct P { char pad[0x13D0]; int x13D0; char pad1[0x13F0 - 0x13D4]; struct O *x13F0; char pad2[0x1450 - 0x13F4]; int x1450; char pad3[0x1530 - 0x1454]; int x1530; };
extern qword D_002C09E0, D_002A2DD0;
void func_001F0660(int a, int b);
void func_001B7D20(struct P *p);
int func_00132B10(struct X *x, int n);
void func_00209EE0(struct X *x);
void func_001F7A68(struct X *x, int n);
qword func_001A6FC0(struct P *p, int n, qword q);
/** @sly2-annot addr=0x1B7BA8
 * Purpose: Abort carrying (on forced state change): detach the carried/lifted object and set its velocity per
 *   current state
 * Info: proposed name DropMurrayCarried (T1); TU murray
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B7BA8(struct P *p)
{
    struct O *o;
    struct X *x;
    if (p->x13D0 == 6) {
        if (p->x1530) func_001F0660(p->x1530, 0x1CA);
        return;
    }
    o = p->x13F0;
    if (!o) return;
    if (p->x13D0 == 4 && p->x1450 == 0) {
        ((void (*)())o->vt[3].pfn)((char *)o + o->vt[3].d, 6, 0, 0, 0, 0);
        p->x13F0 = 0;
        return;
    }
    func_001B7D20(p);
    x = p->x13F0->x180;
    if (func_00132B10(x, 0x38)) func_00209EE0(x);
    else func_001F7A68(x, 1);
    switch (p->x13D0) {
    case 5: {
        struct VE *e = &x->vt[35];
        ((void (*)(void *, qword))e->pfn)((char *)x + e->d, func_001A6FC0(p, 0, D_002C09E0));
        break;
    }
    case 2: case 4: case 7: case 8:
        ((void (*)(void *, qword))x->vt[35].pfn)((char *)x + x->vt[35].d, D_002A2DD0);
        break;
    case 3: case 6:
        break;
    }
    p->x13F0 = 0;
}
