typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z; } V3;
typedef struct {
    char pad0[0x20];
    float x20[3];
    char pad2c[4];
    float x30[3];
    char pad3c[0x50 - 0x3C];
    qword x50;
} DEF;
typedef struct {
    char pad0[0x18];
    float x18;
    char pad1c[4];
    qword x20;
    qword x30;
    char pad40[0x90 - 0x40];
    V3 x90;
    char pad9c[0xF0 - 0x9C];
    DEF *xF0;
    char padf4[0x100 - 0xF4];
    qword x100;
} OBJ;
extern char D_002DDF40[];
extern float D_002C2000;
extern qword D_002BBB50;
void func_001CE920(OBJ *, qword);
qword func_0014F308(void *, qword);
float func_0011D448(float, float);
/** @sly2-annot addr=0x1D04E8
 * Purpose: SMACK (RIPT 9) Init
 * Info: proposed name InitSmack (T1); TU rip
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001D04E8(OBJ *p, qword q)
{
    DEF *d;
    float x, y;
    func_001CE920(p, q);
    d = p->xF0;
    p->x100 = func_0014F308(D_002DDF40, q);
    x = func_0011D448(d->x20[0], d->x30[0]);
    y = func_0011D448(d->x20[1], d->x30[1]);
    {
        float z = func_0011D448(d->x20[2], d->x30[2]);
        V3 *v = &p->x90;
        v->x = x;
        v->y = y;
        v->z = z;
    }
    p->x18 = D_002C2000;
    p->x30 = D_002BBB50;
    p->x20 = d->x50;
}
