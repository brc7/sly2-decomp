typedef int ti128 __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct Q { char pad[0x18]; int f18; };
struct S { int pad[3]; struct VTE *vt; char pad2[0x1e8 - 0x10]; struct Q *f1e8; };
extern ti128 D_002A2DD0;
void func_001A58D0(struct S *, struct Q *, int);
void func_001F4898(struct S *, ti128);
void func_001F4930(struct S *, ti128);
void func_001F5720(struct S *, int);
void func_001F57C0(struct S *, int);
/** @sly2-annot addr=0x13B250
 * Purpose: BOMB vt77: remember thrower (+0x1E8), reparent, zero velocities, lock force/torque constraints,
 *   fire Splice event 0x1B
 * Info: proposed name ApplyBombThrow (T1); class BOMB slot77; TU bomb
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0,
 *   docs/reference/functions/gameplay-objects.md#missile-0x1b1db8-0x1b2820
 * @end-sly2-annot */
void func_0013B250(struct S *p, struct Q *q) {
    p->f1e8 = q;
    func_001A58D0(p, q->f18 != 1 ? q : 0, 3);
    func_001F4898(p, D_002A2DD0);
    func_001F4930(p, D_002A2DD0);
    func_001F5720(p, 1);
    func_001F57C0(p, 1);
    ((void (*)(void *, int, int, int, int, int))p->vt[3].pfn)((char *)p + p->vt[3].d, 0x1b, 0, 0, 0, 0);
}
