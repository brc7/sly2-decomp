typedef int qword __attribute__((mode(TI)));
extern struct { float t; float dt; } D_002DDED0;
extern char D_002DDF40[];
typedef struct { qword a; qword b; } Q2;
typedef struct {
    char pad0[0x14];
    float x14;
    float x18;
    char pad1c[0x70 - 0x1C];
    qword x70;
    qword x80;
    qword x90;
    char pada0[0xF0 - 0xA0];
    Q2 *xF0;
    char padf4[0x100 - 0xF4];
    qword x100;
} OBJ;
void EvaluateBezierPos(float, float, float, qword, qword, qword, qword, qword *, qword *, int);
qword func_0014F268(void *, qword);
static inline float GMax(float a, float b) { return a < b ? b : a; }
void func_001D05A0(OBJ *p)
{
    EvaluateBezierPos(GMax(p->x14 + p->x18 - D_002DDED0.t, D_002DDED0.dt), D_002DDED0.dt, 1.0f, p->x100, p->x90, p->xF0->a, p->xF0->b, &p->x100, &p->x90, 0);
    p->x80 = p->x70;
    p->x70 = func_0014F268(D_002DDF40, p->x100);
}
