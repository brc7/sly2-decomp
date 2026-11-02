typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) VEC;
struct G3 { float x, y, z; };
extern int D_003E12C8;
extern float D_002C3690, D_002C3694, D_002C3698;
extern char D_002BBB60[];
float powf(float, float);
float GLimitLm__FP2LMf(void *, float);
void ConvertUserRgbToUserHsv(struct G3 *, VEC *);
/** @sly2-annot addr=0x1ED2D8
 * Purpose: Adjust a color vector: luminance (0.3,0.6,0.1) through a gamma curve (powf, LM limits D_002C369x)
 *   and rebuild via HSV
 * Info: proposed name ColorCorrectVector (T2); TU shd
 * Notes: names_v2 C proposed ReferenceShaderAqwRegs__FP3SHDP4SHDPP2QWiiP3SAI (0.638)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6b_rest
 * See: docs/engine/rendering.md#8-blips-hud-screen-effects,
 *   docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void func_001ED2D8(VEC *p)
{
    struct G3 c;
    VEC w;
    VEC *pw;
    qword d, one;
    float dot;
    if (!D_003E12C8) return;
    pw = &w;
    w.x = 0.3f;
    pw->y = 0.6f;
    pw->z = 0.1f;
    c.x = D_002C3690;
    c.y = D_002C3694;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(*(qword *)p), "j"(*(qword *)&w));
    __asm__("mtc1 %1, %0" : "=f"(dot) : "r"(d));
    c.z = GLimitLm__FP2LMf(D_002BBB60, powf(dot, D_002C3698));
    ConvertUserRgbToUserHsv(&c, p);
}
