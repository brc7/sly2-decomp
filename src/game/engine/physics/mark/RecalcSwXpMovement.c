typedef int qword __attribute__((mode(TI)));
struct LO;
struct P;
typedef struct { short d; short pad; void (*pfn)(struct LO *, struct P *, int); } VT;
struct LO { int x0, x4, x8; VT *vt; };
struct E { int x0; void *x4; int x8; struct LO *xC; qword x10; qword x20; qword x30; };
struct P { struct E e[2]; char pad[0x30]; qword xB0; };
qword GetXfVelocity(void *p, int n, qword q);
/** @sly2-annot addr=0x1A7FC0
 * Purpose: For each XP recompute the relative velocity at the contact (GetXfVelocity of both sides)
 * Info: proposed name RecalcSwXpMovement (T1); TU mark
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4b_rest+verdicts_A2+verify_side
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
void RecalcSwXpMovement(void *unused, int n, struct P **ap)
{
    int i;
    for (i = 0; i < n; i++) {
        struct P *p = ap[i];
        struct E *e;
        int j;
        e = p->e;
        for (j = 0; j < 2; j++, e++) {
            if (e->x4) {
                struct LO *o;
                e->x30 = GetXfVelocity(e->x4, 0, e->x10);
                o = e->xC;
                o->vt[56].pfn((struct LO *)((char *)o + o->vt[56].d), p, j);
            }
        }
        for (j = 0; j < 2; j++) {
            struct LO *o = p->e[j].xC;
            if (o)
                o->vt[57].pfn((struct LO *)((char *)o + o->vt[57].d), p, j);
        }
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(p->xB0) : "j"(p->e[0].x30), "j"(p->e[1].x30));
    }
}
