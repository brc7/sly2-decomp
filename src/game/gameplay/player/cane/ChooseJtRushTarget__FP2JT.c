typedef struct { float x, y, z, w; } __attribute__((aligned(16))) VEC;
struct T { char pad[0x18]; float g18; float g1c; };
struct S { char pad[0x150]; struct T *pt; char p1[0x54]; float g; char p2[0x1714]; int n; };
extern char D_002BF4D0[];

void ChooseJtAttackTarget(struct S *p, float g, int a, void *pv, int *pn, VEC *pq, float h, float k);

float atan2f(float y, float x);
void func_00204660(struct S *p, float g);
void FixStepAngularVelocity(struct S *p);
/** @sly2-annot addr=0x18F688
 * Purpose: ChooseJtAttackTarget(grf 8, 0.25, 1.0) and turn toward the found target (atan2),
 *   FixStepAngularVelocity
 * Info: proposed name ChooseJtRushTarget (T1); TU cane
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4b_rest+verdicts_B
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#cane-0x18e540-0x18f908
 * @end-sly2-annot */
void ChooseJtRushTarget__FP2JT(struct S *p)
{
    VEC v;
    int n;
    float g;
    if (p->pt && 0.5f < p->pt->g1c) g = p->pt->g18;
    else g = p->g;
    ChooseJtAttackTarget(p, g, 8, D_002BF4D0, &n, &v, 0.25f, 1.0f);
    p->n = n;
    if (n)
        g = atan2f(v.y, v.x);
    func_00204660(p, g);
    FixStepAngularVelocity(p);
}
