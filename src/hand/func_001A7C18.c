typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct LO;
struct C;
typedef struct { short d; short pad; void (*pfn)(struct LO *, struct C *, int); } VTE;
struct SUB { char pad[0x60]; void *x60; };
struct LO { int x0, x4, x8; VTE *vt; char pad[0xEC - 0x10]; struct SUB *xEC; };
struct S { void *x0; struct LO *x4; };
struct E { struct S *ps; void *x4; void *x8; struct LO *plo; qword pos; qword x20; int pad[4]; };
struct C { struct E ae[2]; qword dir; float g; float x94, x98; int x9C, xA0, xA4; int xA8; struct C *next; };
struct W { char pad[0x3D40]; char pool[1]; };
extern float D_002BFD30, D_002BFD34;
struct C *PvAllocSlotheapClear(void *pool);
qword func_0022FC48(int n, void *p, qword pos);
void func_001A7FC0(struct W *pw, int n, struct C **ppc);
/** @sly2-annot addr=0x1A7C18
 * Purpose: Allocate an XP contact (slotheap) between two objects at pos/normal with distance g and kind,
 *   convert to local frames, compute movement, link into *ppxpFirst
 * Info: proposed name PxpAdd (T1); TU mark
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4b_rest+verdicts_A2
 * See: docs/engine/main-loop-startup.md#7-source-files,
 *   docs/engine/physics-collision.md#2-contacts-and-solving, docs/engine/physics-collision.md#6-source-files
 * @end-sly2-annot */
void func_001A7C18(struct W *pw, struct S *a, struct S *b, qword pos, qword dir, int k, int x, int y, struct C **ppc, float g)
{
    U u;
    struct C *c;
    int i;
    struct E *e;
    struct C *pc;
    c = pc = PvAllocSlotheapClear(pw->pool);
    pc->ae[0].pos = pos;
    {
        qword s, r;
        __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(-g));
        u.q = s;
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(pos), "j"(dir), "j"(s));
        pc->ae[1].pos = r;
    }
    pc->dir = dir;
    for (i = 0; i < 2; i++) {
        e = &c->ae[i];
        if (i == 0)
            e->ps = a;
        else
            e->ps = b;
        if (e->ps) {
            e->x4 = e->ps->x0;
            e->plo = e->ps->x4;
            e->x8 = e->plo->xEC->x60;
        }
        e->x20 = func_0022FC48(0, e->x4, e->pos);
    }
    c->g = g;
    c->x9C = k;
    c->x94 = D_002BFD30;
    c->x98 = D_002BFD34;
    c->xA0 = x;
    c->xA4 = y;
    for (i = 0; i < 2; i++) {
        struct LO *plo;
        e = &c->ae[i];
        plo = e->plo;
        if (plo)
            plo->vt[55].pfn((struct LO *)((char *)plo + plo->vt[55].d), c, i);
    }
    func_001A7FC0(pw, 1, &c);
    {
        struct C *t = c;
        while (t->next)
            t = t->next;
        t->next = *ppc;
        *ppc = c;
    }
}
