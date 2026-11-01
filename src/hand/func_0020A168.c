typedef int qword __attribute__((mode(TI)));
struct K { char pad[0x50]; int n50; };
struct E { char pad[0x14]; struct K *pk; char pad2[0x20 - 0x18]; qword pos; char pad3[0x4C - 0x30]; float f4C; int n54x; };
struct E2 { char pad[0x14]; struct K *pk; char pad2[0x20 - 0x18]; qword pos; char pad3[0x4C - 0x30]; float f4C; char pad4[4]; int n54; char pad5[0x60 - 0x58]; };
qword func_001A62E8(void *p);

static inline float DistSq(void *p, qword *pq)
{
    qword a = func_001A62E8(p), d, r, t;
    float f;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(*pq));
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(r), "=&j"(t) : "0"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(r));
    return f;
}

/** @sly2-annot addr=0x20A168
 * Purpose: Compare stimuli: template priority, level, time, then distance
 * Info: proposed name IcmpStepguardStimulus (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_0020A168(void *p, struct E2 *a, struct E2 *b)
{
    int n;
    float d, d2;
    n = a->pk->n50 - b->pk->n50;
    if (n)
        return n;
    n = b->n54 - a->n54;
    if (n)
        return n;
    if (a->f4C > b->f4C)
        return -1;
    if (a->f4C < b->f4C)
        return 1;
    d = DistSq(p, &a->pos);
    d2 = DistSq(p, &b->pos);
    if (d < d2)
        return -1;
    if (d2 > d)
        return 1;
    return a - b;
}
