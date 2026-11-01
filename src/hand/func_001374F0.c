typedef int qword __attribute__((mode(TI)));
struct Q { char pad[0x18]; float g18; float g1C; };
struct O { char pad[0x150]; struct Q *pq; char pad2[0x10DC - 0x154]; void *p10DC; };
struct P { struct O *po; int x4; int x8; int xC; };
qword func_0011E1A0(float a, float b, float c);
qword func_001A62E8(void *p);
/** @sly2-annot addr=0x1374F0
 * Purpose: aim toward +0x10DC target
 * Info: proposed name JtHelper1374F0 (T2); TU bentley
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
int func_001374F0(struct P *p)
{
    float zero = 0.0f;
    struct Q *pq = p->po->pq;
    if (zero < pq->g1C && p->po->p10DC) {
        qword v = func_0011E1A0(pq->g18 - 1.5707964f, 1.0f, zero);
        qword a = func_001A62E8(p->po->p10DC);
        qword b, d, t;
        float g;
        __asm__("" : "+j"(a));
        b = func_001A62E8(p->po);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
        __asm__("vmul.xyz %0, %2, %3\n"
            "vaddw.x %1, $vf0, $vf0w\n"
            "vadday.x ACC, %0, %0y\n"
            "vmaddz.x %0, %1, %0z" : "=j"(d), "=j"(t) : "j"(v), "j"(d));
        __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
        if (g > zero) return 0; return 1;
    }
    return p->xC;
}
