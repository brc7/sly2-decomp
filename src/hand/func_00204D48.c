typedef struct VECTOR { float x, y, z; } VECTOR;
extern VECTOR D_002C3CB8;
static __inline__ float SqrtF(float g) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=f"(r) : "f"(g)); return r; }
struct S { char pad[0x1F0]; float g; VECTOR v; };
/** @sly2-annot addr=0x204D48
 * Purpose: smpSpin = default D_002C3CB8 with svFast/svSlow *r and dtFast /sqrt(r); store r at +0x1F0
 * Info: proposed name SetStepSpinRatio (T1); TU step
 * Notes: name currently held by 0x1B17B8, which existing_name_changes.csv proposes to rename
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions, docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void func_00204D48(struct S *p, float g)
{
    p->v = D_002C3CB8;
    p->v.x *= g;
    p->v.y *= g;
    p->v.z *= 1.0f / SqrtF(g);
    p->g = g;
}
