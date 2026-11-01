typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct VT { short d; short pad; void *pfn; };
struct LO { char pad[0xC]; struct VT *vt; };
struct P { char pad[0x150]; float g150; float g154; char pad2[0x220 - 0x158]; int n220; };
extern char D_003DD490[];
qword func_001A62E8(void *p);
struct LO *func_001C4AF8(void *pg, qword pos);
float func_002458F0(float y, float x);
void func_001B1818(struct P *p, float g);
static inline float Length(qword v)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
/** @sly2-annot addr=0x1B1900
 * Purpose: Look at nearest player (0x1C4AF8): flee/turn decision and heading
 * Info: proposed name ChooseCid96StateFromPlayer (T2); class CID96; TU tu_1B0B78
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
int func_001B1900(struct P *p)
{
    int r = p->n220;
    qword pos = func_001A62E8(p);
    struct LO *po = func_001C4AF8(D_003DD490, pos);
    U d;
    float len;
    if (!po)
        return r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d.q) : "j"(func_001A62E8(po)), "j"(pos));
    len = Length(d.q);
    if (p->g154 < len)
        return r;
    if (p->g150 < len) {
        qword v = ((qword (*)(void *))po->vt[33].pfn)((char *)po + po->vt[33].d);
        if (Length(v) < 150.0f)
            return r;
    }
    func_001B1818(p, func_002458F0(d.f[1], d.f[0]));
    return 3;
}
