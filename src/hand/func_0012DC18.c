typedef int qword __attribute__((mode(TI)));
typedef struct R { char pad[0x1C]; int x1C; qword v; } R;
R *func_0012DB50(int k, void *a, void *b, void *c, int d);
void func_00230018(void *a, void *b, void *p);
/** @sly2-annot addr=0x12DC18
 * Purpose: Create an ACT of kind 1 (PactNewKind) for (so, xf), store an int (+0x1C) and a vector goal
 *   (+0x20), insert it into the XF actuator list (AddXfAct), return it
 * Info: proposed name PactNewPosition (T2); TU act
 * Notes: v3: superseded PactNewVector | top50_round (0.50): Body. +0x20 is the vector SetActPositionGoal
 *   0x12D108 later updates, so this is the position-goal actuator constructor; siblings PactNewA..E
 *   0x12DBB0..0x12DE58 build other kinds. Callers: PostJtLoad, PostMurrayLoad, Atta
 * Confidence: low (draft) [T2 0.50] - evidence: semantic review; sources: r1_116918+top50_round
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600,
 *   docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8,
 *   docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
R *func_0012DC18(void *a, void *b, void *c, int d, int e, qword v)
{
    R *p = func_0012DB50(1, a, b, c, d);
    p->x1C = e;
    p->v = v;
    func_00230018(a, b, p);
    return p;
}
