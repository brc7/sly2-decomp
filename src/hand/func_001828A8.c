typedef int qword __attribute__((mode(TI)));
struct Q { qword pos; qword v10; char pad[0x4C - 0x20]; int n4C; char pad2[0x58 - 0x50]; float g58; float g5C; };
struct M { char pad[0x10]; int n10; char pad1[0x20 - 0x14]; qword v20; qword v30; char pad2[0x44 - 0x40]; int n44; };
struct O { char pad[0x190]; void *p190; };
extern struct Q *func_00182810(struct O *po, qword pos);
extern float GRandInRange__Fff(float a, float b);
extern void func_00181EB0(struct O *po, struct Q *pq, int n);
extern void func_0015C198(void *p, int k, int n, qword a, qword b, float g, float h);
extern float D_002BE8A8;
extern qword D_002A2DC0;
extern qword D_002A2DD0;
int func_001828A8(struct O *po, struct M *pm)
{
    struct Q *pq;
    if (!(pm->n44 & 8))
        return 0;
    pq = func_00182810(po, pm->v20);
    if (pq) {
        pq->n4C += pm->n10;
        if (pq->n4C > 0) {
            qword c, d, one;
            float g;
            __asm__("vopmula.xyz ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(c) : "j"(D_002A2DC0), "j"(pq->v10));
            __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(pm->v30), "j"(c));
            __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
            pq->g58 = g * D_002BE8A8;
            pq->g5C = GRandInRange__Fff(-15.0f, 15.0f);
            func_00181EB0(po, pq, 1);
        } else {
            if (po->p190)
                func_0015C198(po->p190, 4, 0, D_002A2DD0, pq->pos, 0.0f, 0.0f);
            func_00181EB0(po, pq, 2);
        }
    }
    return 1;
}
