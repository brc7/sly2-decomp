// MATCH (Claude agent, lane D24, 2026-10-23): v6 with the mtc1 asm's allocation-only "j" operands replaced by an
// "r,j" constraint on td (never-chosen alternative, forced off by an unused "i,r"(0) operand). With fewer operands
// the mtc1 has 4 dependents like the `one` sqc2, so the luid tie-break issues the sqc2 first, like the target.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct PL { qword pos; qword plane; };
struct P { char pad[0x48]; float g48; };
extern float D_002BD8D8;
extern char D_002BD8E0[];
extern float D_002DDEE0;
void *func_002564B8(void *p);
void GSmooth__FfffP3SMPPf(float g, float gTarget, float dt, void *pv, float *pr);
static __inline__ float GDistPlane(qword plane, qword pos)
{
    U d, one;
    qword td, tone;
    float dist;
    func_002564B8(&d);
    func_002564B8(&one);
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %2w" : "=&j"(td), "=&j"(tone) : "j"(plane), "j"(pos));
    /* "r,j"(td): the j alternative is allocation-only (keeps td a VF reg, so it is stored with sqc2 and moved with
       qmfc2 v0); the unused "i,r"(0) operand makes that alternative always need a reload, so reload picks "r".
       Only td is read, so the asm has no anti-dependence on the later vf1 load and the sqc2 of `one` wins the tie. */
    __asm__("mtc1 %1, %0" : "=f,f"(dist) : "r,j"(td), "i,r"(0));
    one.q = tone;
    d.q = td;
    return dist;
}
static __inline__ float GDot(const qword *pa, qword b)
{
    qword a = *pa;
    qword d, one;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
float func_0014F040(struct P *p, int unused, qword vIn, struct PL *pl)
{
    float r;
    qword plane = pl->plane;
    qword pos = pl->pos;
    GSmooth__FfffP3SMPPf(GDistPlane(plane, pos), p->g48 - D_002BD8D8, D_002DDEE0, D_002BD8E0, &r);
    return GDot(&vIn, pl->plane) - r;
}
