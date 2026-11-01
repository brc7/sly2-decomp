typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x40 - 0x10]; int n40; char pad3[0x64 - 0x44]; float g64; char pad4[0xCC - 0x68]; int nCC; float gD0; };
struct P { char pad[0x10]; void *o10; char pad2[0x30 - 0x14]; int n30; float g34; int n38; float g3C; qword q40; };
void func_001EEF70(void *p);
struct O *func_00205438(void *p, int n);
void func_00144F48(struct P *p, qword q);
/** @sly2-annot addr=0x144E68
 * Purpose: BHV193 slot 8: push a BHV168 (0x205438(owner,0xA8)) with own parameters and goal, start it
 * Info: proposed name StartBhv193 (T2); class BHV193 slot8; TU tu_144B28
 * Notes: superseded: OnBhv193Resume (r2b_rest) | BHV vtable slot 8 role per r9 (CONFLICTS #15); was
 *   OnBhv193Resume
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00144E68(struct P *p)
{
    struct O *o;
    struct VTE *e;
    func_001EEF70(p);
    o = func_00205438(p->o10, 0xA8);
    o->n40 = p->n30;
    o->g64 = p->g34;
    o->nCC = p->n38;
    o->gD0 = p->g3C;
    func_00144F48(p, p->q40);
    e = &o->vt[8];
    ((void (*)(void *))e->pfn)((char *)o + e->d);
}
