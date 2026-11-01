typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) VEC;
struct G3 { float x, y, z; };
extern int D_003E12C8;
extern float D_002C3690, D_002C3694, D_002C3698;
extern char D_002BBB60[];
float func_00245B58(float, float);
float func_0011D948(void *, float);
void ConvertUserRgbToUserHsv(struct G3 *, VEC *);
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
    c.z = func_0011D948(D_002BBB60, func_00245B58(dot, D_002C3698));
    ConvertUserRgbToUserHsv(&c, p);
}
