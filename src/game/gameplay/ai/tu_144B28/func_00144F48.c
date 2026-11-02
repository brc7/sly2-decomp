typedef int qword __attribute__((mode(TI)));
struct E;
struct D { char pad[0x10]; struct E *pe; };
struct C { char pad[0xDC]; struct D *pd; };
struct B { char pad[0xE8]; int k; char pad2[0xF4 - 0xEC]; qword *pq; };
struct A { char pad[0xE8]; struct C *pc; char pad2[0x294 - 0xEC]; struct B *pb; };
struct P { char pad[0x10]; struct A *pa; char pad1[4]; void *p18; char pad2[0x30 - 0x1C]; int f30; char pad3[0x40 - 0x34]; qword v; };
qword func_001A62E8(struct A *pa);
void func_001C02D0(struct E *pe, qword r, qword v, struct B *pb);
void MeasureShape(struct B *pb);
void SetBhvpathShape(void *p, struct B *pb, int n);
/** @sly2-annot addr=0x144F48
 * Purpose: store goal; build a path (straight or via nav graph 0x1C02D0) into the child's route
 * Info: proposed name SetBhv193Goal (T2); TU tu_144B28
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00144F48(struct P *p, qword v)
{
    p->v = v;
    if (p->p18) {
        struct A *pa = p->pa;
        struct B *pb = pa->pb;
        struct D *pd = pa->pc ? pa->pc->pd : 0;
        struct E *pe = pd ? pd->pe : 0;
        qword r = func_001A62E8(pa);
        if (!pe || p->f30) {
            pb->k = 2;
            pb->pq[0] = r;
            pb->pq[1] = p->v;
            MeasureShape(pb);
        } else {
            func_001C02D0(pe, r, p->v, pb);
        }
        SetBhvpathShape(p->p18, pb, 0);
    }
}
