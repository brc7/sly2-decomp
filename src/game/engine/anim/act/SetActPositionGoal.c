typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
int FFloatsNear__Ffff(float a, float b, float eps);
void SlotUnsettle(void *pv, void *px);
struct X { char pad[0x90]; void *p90; char pad2[0xC0 - 0x94]; qword vC0; };
struct P { int x0; void *x4; struct X *x8; int xC; int pad[4]; qword v20; };
extern qword D_002A2DD0;
static __inline__ int FVecsNear(qword a, qword b)
{
    return FFloatsNear__Ffff(GetX(a), GetX(b), 1e-4f) && FFloatsNear__Ffff(GetY(a), GetY(b), 1e-4f) &&
           FFloatsNear__Ffff(GetZ(a), GetZ(b), 1e-4f);
}
/** @sly2-annot addr=0x12D108
 * Purpose: Store a new position goal (+0x20); if this act is the XF driver (xf+0x90==act) and the goal moved,
 *   copy it to xf+0xC0 and un-settle the slot (0x230598)
 * Info: proposed name SetActPositionGoal (T1); TU act
 * Notes: top50_round (0.65): Body. Pairs with SetActRotationGoal 0x12DA30; Sly 1 ACT has
 *   GetActPositionGoal/GetActRotationGoal. Callers UpdateJtHands, UpdateJtHangIkGoals, UpdateMurray,
 *   UpdateRobottank
 * Confidence: mid (draft) [T1 0.65] - evidence: Sly 1 / Lilly reference; semantic review; sources:
 *   r1_116918+top50_round
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/animation.md#act-0x12b3b0-0x12e600,
 *   docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void SetActPositionGoal(struct P *p, qword v)
{
    qword dv;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(dv) : "j"(v), "j"(p->v20));
    p->v20 = v;
    if (p->x8->p90 == p && !FVecsNear(D_002A2DD0, dv)) {
        p->x8->vC0 = v;
        SlotUnsettle(p->x4, p->x8);
    }
}
