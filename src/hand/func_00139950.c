// laneZ2 v4 (Claude agent, 2026-10-30): particle update through VU0 microprogram 0x10.
// Built from build/hand_wip/func_00139950.lilly1.c (Claude agent for Lilly, T1646). The fixed vf21-vf31 registers
// of the microprogram are bound only inside the inline VU helper UpdateMs10, which loads them, runs vcallms 0x10
// per live particle and stores the results after the cfc2.i interlock; the function body has no register pins.
// Microprogram ABI: vf27 = dt, vf28..vf31 = matrix rows 3..0, vf23/vf24 = the pa/pb entries; it accumulates the
// bounds in vf26 (min) / vf25 (max) in place and writes the new pa/pb entries to vf21/vf22.
// vmin/vmax are only asm INPUTS of the vcallms: keeping both registers set once makes sched1 treat their loads as
// "birthing" insns, which gives the original order of the parameter spill stores (see notes/130000.md).
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
typedef struct { float x, y, z, w; } V;
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
float RadNormalize__Ff(float);

static __inline__ float FloatFromQw(qword q)
{
    float g;
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q));
    return g;
}

/* Non-VU work done while the microprogram runs: move particle i's extra entry down to slot j. */
static __inline__ void MoveExtra(qword *pc, int i, int j, int k, qword dt)
{
    pc[j] = pc[i];
    if (k == 2) {
        float g = FloatFromQw(dt);
        ((V *)&pc[j])->w = RadNormalize__Ff(((V *)&pc[i])->w + ((V *)&pc[i])->z * g);
    }
}

static __inline__ int UpdateMs10(const M4 *pm, qword dt, qword *pa, qword *pb, qword *pc, int c, int k,
                                 qword *pvMin, qword *pvMax)
{
    register qword vdt __asm__("vf27") = dt;
    register qword m0 __asm__("vf31") = pm->r[0];
    register qword m1 __asm__("vf30") = pm->r[1];
    register qword m2 __asm__("vf29") = pm->r[2];
    register qword m3 __asm__("vf28") = pm->r[3];
    register qword vmin __asm__("vf26") = *pvMin;
    register qword vmax __asm__("vf25") = *pvMax;
    register qword v21 __asm__("vf21");
    register qword v22 __asm__("vf22");
    register qword v23 __asm__("vf23");
    register qword v24 __asm__("vf24");
    float t = D_002DDED0.t;
    int cDead = 0;
    int j = -1;
    int i;
    int sync;

    for (i = 0; i < c; i++) {
        if (((V *)&pb[i])->w <= t) {
            cDead++;
            continue;
        }
        v24 = pb[i];
        v23 = pa[i];
        __asm__ __volatile__("vcallms 0x10" : "=j"(v21), "=j"(v22)
            : "j"(v23), "j"(v24), "j"(vdt), "j"(m0), "j"(vmax), "j"(vmin));
        j++;
        MoveExtra(pc, i, j, k, dt);
        __asm__ __volatile__("cfc2.i %0, $vi0" : "=r"(sync) : "j"(v21), "j"(v22), "j"(m1), "j"(m2), "j"(m3));
        pb[j] = v22;
        pa[j] = v21;
    }
    *pvMin = vmin;
    *pvMax = vmax;
    return cDead;
}

int func_00139950(M4 *pm, qword dt, qword *pa, qword *pb, qword *pc, int c, int k, qword *pvMin, qword *pvMax)
{
    return UpdateMs10(pm, dt, pa, pb, pc, c, k, pvMin, pvMax);
}
