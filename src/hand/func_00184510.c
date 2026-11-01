typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } VEC;
typedef union { qword q; VEC v; } UV;
struct S { char pad[0x110]; qword q110; char p1[0x3C]; void *p15c; };
extern int D_002DFB84;
extern float D_002BE908;
qword func_001A62E8(void *p);
qword func_001A6F00(struct S *p, int n, qword q);
static __inline__ float GetZ(qword q)
{
    float z;
    __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q));
    return z;
}
/** @sly2-annot addr=0x184510
 * Purpose: Hold position (debug flag D_002DFB84 uses child x15C pos + offset)
 * Info: proposed name GetCid27GrabPos (T2); class CID27; TU grabspot
 * Notes: names_v2 B proposed PresetHbskAccel__FP4HBSKf (0.856)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00184510(struct S *p, int unused, UV *pv)
{
    if (pv) {
        if (D_002DFB84) {
            pv->q = func_001A62E8(p->p15c);
            pv->v.z = pv->v.z + D_002BE908;
        } else {
            pv->q = func_001A6F00(p, 0, p->q110);
            pv->v.z = GetZ(func_001A62E8(p->p15c)) + D_002BE908;
        }
    }
}
