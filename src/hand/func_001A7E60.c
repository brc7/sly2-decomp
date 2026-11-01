/* Update two contact positions and their separation projected onto the contact axis. */
typedef int qword __attribute__((mode(TI)));
struct E0 { int x0, x4; float x8; };
struct LO { char pad[0x70]; qword x70; };
struct E { struct E0 *x0; struct LO *x4; int x8; int xC; qword x10; qword x20; qword x30; };
struct P { struct E e[2]; qword x80; float x90; int pad[2]; int x9C; };
qword func_0022FC48(struct LO *p, struct LO *other, qword q);
static __inline__ qword Lin(qword a, qword *pb, int f)
{
    union { qword q; float f; } t;
    __asm__("qmtc2.ni %1, %0" : "=j"(t.q) : "r"(f));
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(a) : "0"(a), "j"(*pb), "j"(t.q):"memory");
    return a;
}
/** @sly2-annot addr=0x1A7E60
 * Purpose: For each XP in the array recompute world positions of its two sides (ConvertXfPos)
 * Info: proposed name RecalcSwXpPos (T1); TU mark
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4b_rest+verdicts_A2+verify_side
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
void func_001A7E60(void *unused, int n, struct P **ap)
{
    int i;
    for (i = 0; i < n; i++) {
        struct P *p = ap[i];
        struct E *e;
        int j;
        e = p->e;
        for (j = 0; j < 2; j++, e++) {
            struct LO *o = e->x4;
            if (!o)
                continue;
            {
            int f; qword position;
            union { float f; int i; } u;
            if (j == 0) {
                switch (p->x9C) {
                case 4: case 5: case 6:
                    u.f = -e->x0->x8; f = u.i; position=o->x70;
                    break;
                default:
                    goto def;
                }
            } else {
                switch (p->x9C) {
                case 7: case 9:
                    position=o->x70; f = *(int *)&e->x0->x8;
                    break;
                default:
                    goto def;
                }
            }
            e->x10 = Lin(position,&p->x80,f);
            continue;
            }
        def:
            e->x10 = func_0022FC48(o, 0, e->x20);
        }
        {
            qword d, r, one;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(p->e[0].x10), "j"(p->e[1].x10));
            __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(r), "=&j"(one) : "j"(p->x80), "j"(d));
            { register float g __asm__("$4"); __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(r)); __asm__("sw %1, %0" : "=m"(p->x90) : "r"(g)); }
        }
    }
}
