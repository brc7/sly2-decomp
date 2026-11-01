typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x10C]; float g10C; };
struct J { char pad[0x240]; qword v240; char pad2[0x103C - 0x250]; float g103C; int n1040; };
extern qword func_001A62E8(void *p);
extern qword func_0011E1A0(float a, float b, float c);
static __inline__ qword VAdd(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static __inline__ qword VSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
/** @sly2-annot addr=0x183540
 * Purpose: Point at angle g/r on the circle of radius r + player radius
 * Info: proposed name GetCid34GrabPos (T2); class CID34; TU grabspot
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00183540(struct O *p, float g, struct J *pj, qword *pv, float *pg)
{
    float gPan = g / p->g10C;
    float gR = p->g10C + pj->g103C;
    if (pv) {
        *pv = VSub(VAdd(func_001A62E8(p), func_0011E1A0(gPan, gR, 0.0f)), pj->v240);
    }
    if (pg) {
        if (pj->n1040 == 1)
            gPan += 3.14159265f;
        *pg = gPan;
    }
}
