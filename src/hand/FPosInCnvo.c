typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct X { char pad[0xE8]; int cplane; char pad2[0x1EC - 0xEC]; qword *aplane; };
void func_001BD400(struct X *p);
void func_002564B8(void *p);
static __inline__ float GDistPlane(qword plane, qword pos)
{
    U d, one;
    qword td, tone;
    float dist;
    func_002564B8(&d);
    func_002564B8(&one);
    __asm__("vmul.xyz %0, %3, %4\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %3w\n\tqmfc2.ni %2, %0" : "=j"(td), "=&j"(tone), "=r"(dist) : "j"(plane), "j"(pos));
    one.q = tone;
    d.q = td;
    return dist;
}
int FPosInCnvo(struct X *p, qword pos)
{
    int i;
    func_001BD400(p);
    for (i = 0; i < p->cplane - 1; i++) {
        if (0.0f < GDistPlane(p->aplane[i], pos))
            return 0;
    }
    return 1;
}
