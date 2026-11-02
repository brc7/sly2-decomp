typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x10C]; int n10C; };
struct J { char pad[0x240]; qword v240; char pad2[0x103C - 0x250]; float g103C; int n1040; };
extern qword func_001A62E8(void *p);
extern qword *func_001A6318(void *p);
extern float atan2f(float y, float x);
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0, %1" : "=r"(y) : "r"(q)); return y; }
/** @sly2-annot addr=0x183818
 * Purpose: Hold position/heading from the object matrix (offset by player radius unless x10C)
 * Info: proposed name GetCid38GrabPos (T2); class CID38; TU grabspot
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00183818(struct O *p, struct J *pj, qword *pv, float *pg)
{
    float g = 0.0f;
    qword d;
    if (p->n10C == 0)
        g = pj->g103C;
    d = *func_001A6318(p);
    if (pv) {
        qword a, s, r;
        union { qword q; float f[4]; } u;
        a = func_001A62E8(p);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(pj->v240));
        __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(g));
        u.q = s;
        __asm__("vadda.xyzw ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(r), "j"(d), "j"(s));
        *pv = r;
    }
    if (pg) {
        float gAng = atan2f(GetY(d), GetX(d));
        if (pj->n1040 == 1)
            gAng += 3.14159265f;
        *pg = gAng;
    }
}
