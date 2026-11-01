typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x38]; float g38; char pad2[0x110 - 0x3C]; qword pos; };
extern void PostLoLoad(struct O *p);
extern qword func_001A62E8(void *p);
extern qword ConvertAloPos(void *p, void *q, qword v);
extern float D_002BE904;
static __inline__ float GDist(qword a, qword b)
{
    qword d, one;
    float g;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
        __asm__("vmul.xyz %0, %1, %1" : "=j"(d) : "j"(d));
        __asm__("vaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(one), "+j"(d));
        __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(d) : "j"(d));
        __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
/** @sly2-annot addr=0x1843F0
 * Purpose: vt15: PostLoLoad; default radius = distance to its point + D_002BE904
 * Info: proposed name PostCid27Load (T2); class CID27 slot15; TU grabspot
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001843F0(struct O *p)
{
    PostLoLoad(p);
    if (p->g38 == 1e10f) {
        p->g38 = GDist(func_001A62E8(p), ConvertAloPos(p, 0, p->pos)) + D_002BE904;
    }
}
