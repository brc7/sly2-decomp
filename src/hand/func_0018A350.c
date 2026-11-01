typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } VEC;
typedef union { qword q; VEC v; } UV;
struct S { char pad[0x18C4]; void *p18c4; int n; };
qword func_001A62E8(void *p);
void func_00141588(struct S *p, UV *pv);
static __inline__ float GetZ(qword q)
{
    float z;
    __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q));
    return z;
}
/** @sly2-annot addr=0x18A350
 * Purpose: JT vt97 (p, out): target pos; jts 3 -> z of x18C4 obj -175, jts 0x16 -> -100, jts 0x15 own pos;
 *   default STEPPLAYER 0x141588
 * Info: proposed name GetJtCameraTarget (T2); class JT slot97; TU jt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018A350(struct S *p, UV *pv)
{
    switch (p->n) {
    case 3:
        func_00141588(p, pv);
        pv->v.z = GetZ(func_001A62E8(p->p18c4)) + -175.0f;
        break;
    case 0x16:
        func_00141588(p, pv);
        pv->v.z = GetZ(func_001A62E8(p->p18c4)) + -100.0f;
        break;
    case 0x15:
        pv->q = func_001A62E8(p);
        break;
    default:
        func_00141588(p, pv);
        break;
    }
}
