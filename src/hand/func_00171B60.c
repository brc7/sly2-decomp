typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct VTE { short d; short pad; void (*pfn)(); };
struct T { char pad[0xC]; struct VTE *vt; char pad2[0x1A0]; char a1B0[0x10]; };
struct S { char pad[0x10]; struct T *pt; };
void func_0020B210(struct T *t);
void func_00204660(struct T *t, float g);
/** @sly2-annot addr=0x171B60
 * Purpose: Places the guard at a position and heading and zeroes its velocities
 * Info: proposed name TeleportBhv169Guard (T2); TU tu_1709A8
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00171B60(struct S *p, qword q, float g) {
    struct T *t = p->pt;
    struct VTE *e;
    func_0020B210(t);
    e = &t->vt[0x1D]; ((void (*)(void *, qword))e->pfn)((char *)t + e->d, q);
    func_00204660(t, g);
    e = &t->vt[0x1E]; ((void (*)(void *, void *))e->pfn)((char *)t + e->d, t->a1B0);
    e = &t->vt[0x23]; ((void (*)(void *, qword))e->pfn)((char *)t + e->d, D_002A2DD0);
    e = &t->vt[0x24]; ((void (*)(void *, qword))e->pfn)((char *)t + e->d, D_002A2DD0);
}
