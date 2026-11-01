typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x10C]; float g10C; };
struct J { char pad[0x240]; qword v240; char pad2[0x103C - 0x250]; float g103C; int n1040; };
extern qword func_001A62E8(void *p);
extern qword SetVectorCylind(float a, float b, float c);
static __inline__ qword VAdd(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static __inline__ qword VSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
void func_00183540(struct O *p, float g, struct J *pj, qword *pv, float *pg)
{
    float gPan = g / p->g10C;
    float gR = p->g10C + pj->g103C;
    if (pv) {
        *pv = VSub(VAdd(func_001A62E8(p), SetVectorCylind(gPan, gR, 0.0f)), pj->v240);
    }
    if (pg) {
        if (pj->n1040 == 1)
            gPan += 3.14159265f;
        *pg = gPan;
    }
}
