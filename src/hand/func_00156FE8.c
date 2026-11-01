typedef int qword __attribute__((mode(TI)));
struct LO { char pad[0x28]; int x28; signed char ips; char x2D; long k : 4; unsigned int pad1 : 8; unsigned int f : 1; };
struct Q { char pad[0x50]; int x50; float g54; int x58; };
struct P { char pad[0x14]; struct Q *pq; char pad2[8]; qword v20; };
extern int func_0011D910(void *p, float z);
extern qword func_001A62E8(struct LO *p);
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q)); return z; }
static __inline__ float DistXY(qword d)
{
    qword l;
    float dist;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=&j"(l) : "j"(d));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    __asm__("qmfc2.ni %0, %1" : "=r"(dist) : "j"(l));
    return dist;
}
static __inline__ int FIn(qword d, float g) { return DistXY(d) < g; }
/** @sly2-annot addr=0x156FE8
 * Purpose: True if an LO lies within a noise's height range and radius
 * Info: proposed name FLoInNoise (T2); TU noise
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
int func_00156FE8(struct LO *plo, struct P *p)
{
    struct Q *pq;
    qword d, pos;
    if (plo->f) {
        pq = p->pq;
        if (!pq->x50) {
            pos = func_001A62E8(plo);
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos), "j"(p->v20));
            if (func_0011D910(&pq->x58, GetZ(d)) && FIn(d, pq->g54))
                return 1;
        }
    }
    return 0;
}
