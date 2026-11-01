typedef int qword __attribute__((mode(TI)));
struct E { qword pos; float g; int pad[3]; };
struct S { qword pos; float g10; unsigned int c; int pad[2]; struct E ae[0x40]; qword pos820; };
struct R { float a, b; };
extern struct R D_002A53C0, D_002A53C8;
void GetVectorSphere(qword v, float *pa, float *pb, float *pc);
int func_0011D910(struct R *pr, float g);
float RadNormalize__Ff(float g);
/** @sly2-annot addr=0x172798
 * Purpose: Adds a candidate point to a 64-entry list if its pitch and distance are within limits, storing its
 *   relative heading
 * Info: proposed name FAddSearchCandidate (T2); TU tu_171F58
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158
 * @end-sly2-annot */
int func_00172798(struct S *p) {
    float a, b, c;
    qword d;
    struct E *pe;
    float f;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(p->pos820), "j"(p->pos));
    GetVectorSphere(d, &a, &b, &c);
    if (!func_0011D910(&D_002A53C0, c)) return 0;
    if (!func_0011D910(&D_002A53C8, b)) return 0;
    if (p->c >= 0x40) return 0;
    pe = &p->ae[p->c++];
    pe->pos = p->pos820;
    f = RadNormalize__Ff(a - p->g10);
    pe->g = f;
    if (f < 0.0f) pe->g = 6.28318530718f - f;
    return 1;
}
