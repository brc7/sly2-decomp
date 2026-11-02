struct S { int a; float b; int c[2]; float d; int e[3]; float f; int g[3]; float h; char pad[0x54]; float i, j; };
extern float D_002A96E0, D_002A96E4;
extern char D_002C41B0[];
static inline float sqrtf_(float x) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(x)); return r; }
float func_002458F0(float y, float x);
float func_0011D948(void *p, float f);
/** @sly2-annot addr=0x220D40
 * Purpose: if both angles are the sentinel, set both to GLimitLm(atan2(h, len))
 * Info: proposed name InitCptnAngles (T2); TU tn
 * Notes: agent qualifier: lazy pitch init
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/camera.md#3-camera-policies-cplcycpp-0x14fca8-0x1520c8-plus-tncpp,
 *   docs/engine/camera.md#4-source-files, docs/engine/camera.md#5-key-functions
 * @end-sly2-annot */
void func_00220D40(struct S *p)
{
    if (p->i == D_002A96E0 && p->j == D_002A96E4) {
        float a = p->b + p->d;
        float f = func_002458F0(p->h, sqrtf_(a * a + p->f * p->f));
        f = func_0011D948(D_002C41B0, f);
        p->j = f;
        p->i = f;
    }
}
