typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x40]; qword q50; char pad3[8]; int n68; char pad4[0x240 - 0x6C]; qword pos; char pad5[0xFE8 - 0x250]; int nFE8; };
struct S { char pad[0x10]; struct O *p10; int pad2; char a18[0x18]; char a30[0x10]; };
void func_001EF0D8(void *p);
qword func_001A62E8(struct O *p);
qword func_001C1EC0(void *p, qword q);
int func_00173610(qword *pq, float a, float b, int n, void *pfn);
void func_0017D608(qword *pq);
struct O *func_00205438(struct O *p, int n);
void func_0020B210(struct O *p);
/** @sly2-annot addr=0x17E9B0
 * Purpose: Snap the point to ground (or the surface above); leap there (capability bit 0) or teleport
 *   (detach, set position, zero velocities)
 * Info: proposed name GoBhv191 (T2); TU guardbhv
 * Notes: BHV class word normalised: GoBhvrelocate -> GoBhv191
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017E9B0(struct S *p) {
    union { qword q; float a[4]; } u;
    func_001EF0D8(p->a18);
    u.q = func_001C1EC0(p->a30, func_001A62E8(p->p10));
    if (!func_00173610(&u.q, 150.0f, -250.0f, 1, 0)) func_0017D608(&u.q);
    if (p->p10->nFE8 & 1) {
        struct O *o = func_00205438(p->p10, 0xAC);
        struct VTE *e;
        o->q50 = u.q;
        o->n68 = 1;
        e = &o->vt[8];
        e->pfn((char *)o + e->d);
    } else {
        func_0020B210(p->p10);
        {
            struct O *o = p->p10;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(u.q) : "j"(u.q), "j"(o->pos));
            o->vt[29].pfn((char *)o + o->vt[29].d, u.q);
        }
        { struct O *o = p->p10; o->vt[35].pfn((char *)o + o->vt[35].d, D_002A2DD0); }
        { struct O *o = p->p10; o->vt[36].pfn((char *)o + o->vt[36].d, D_002A2DD0); }
    }
}
