struct VECTOR3 { float x, y, z; };
extern VECTOR3 D_002C0248;
static inline float SqrtF(float g) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=f"(r) : "f"(g)); return r; }
struct STEP { char pad[0x210]; VECTOR3 v; float r; };
/** @sly2-annot addr=0x1B17B8
 * Purpose: Store scale vector D_002C0248*(r, r, 1/sqrt r) at +0x210 and r at +0x21C
 * Info: proposed name SetCid96SpinRatio (EXISTING); class CID96; TU tu_1B0B78
 * Notes: agent/verifier proposes SetCid96SquashRatio for this existing name (see existing_name_changes.csv) |
 *   RENAME proposed -> SetCid96SpinRatio (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.65] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r5b_rest+arbitration
 * See: docs/engine/gameplay-objects.md#gameplay-object-classes,
 *   docs/engine/gameplay-objects.md#key-functions, docs/engine/player.md#3-step-stepcpp-0x203948
 * @end-sly2-annot */
void SetStepSpinRatio(STEP *pstep, float r)
{
    pstep->v = D_002C0248;
    pstep->v.x *= r;
    pstep->v.y *= r;
    pstep->v.z *= 1.0f / SqrtF(r);
    pstep->r = r;
}
