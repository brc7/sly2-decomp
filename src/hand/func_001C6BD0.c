typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pf)(); };
struct V3 { float x, y, z; };
struct X { char pad[0xB0]; struct V3 v; };
struct CLOCK { float t, dt; };
extern struct CLOCK D_002DDED0;
extern char D_002A2D00[];
extern const float D_002A7758;
struct S { char pad[0xC]; struct VTE *vt; char p1[0x58 - 0x10]; struct X *x58; struct X *x5C; char p2[0x160 - 0x60]; float x160;
    char p3[0x180 - 0x164]; qword x180; char p4[0x1C4 - 0x190]; int x1C4; int x1C8; float x1CC; };
void func_001A58D0(struct S *p, int a, int b);
static inline void Zero(struct V3 *pv)
{
    pv->z = 0.0f;
    pv->y = 0.0f;
    pv->x = 0.0f;
}
#define VT(o, n) (o)->vt[n]
/** @sly2-annot addr=0x1C6BD0
 * Purpose: re-parent spawned prize (0x1A58D0) and init motion
 * Info: proposed name LaunchSprayedDprize (T1); TU prize
 * Notes: verifier PLAUSIBLE: AttachSprayedDprize -> LaunchSprayedDprize | names_v2 C proposed
 *   PostAlarmLoad__FP5ALARM (0.167)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0,
 *   docs/reference/functions/gameplay-objects.md#tu_1c2d20-0x1c2d20-0x1c3390
 * @end-sly2-annot */
void func_001C6BD0(struct S *p, qword q1, qword q2, float f1, float f2)
{
    ((void (*)(void *))VT(p, 8).pf)((char *)p + VT(p, 8).d);
    func_001A58D0(p, 0, 0);
    ((void (*)(void *, qword))VT(p, 29).pf)((char *)p + VT(p, 29).d, q1);
    ((void (*)(void *, void *))VT(p, 30).pf)((char *)p + VT(p, 30).d, D_002A2D00);
    Zero(&p->x58->v);
    Zero(&p->x5C->v);
    p->x160 = f1;
    p->x1C4 = 0;
    ((void (*)(void *, int))VT(p, 76).pf)((char *)p + VT(p, 76).d, 3);
    p->x180 = q2;
    p->x1CC = (f2 == D_002A7758) ? D_002A7758 : D_002DDED0.t + f2;
}
