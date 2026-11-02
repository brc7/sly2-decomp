typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0, %1" : "=r"(y) : "r"(q)); return y; }
extern float D_002C0AB0;
/** @sly2-annot addr=0x1BC278
 * Purpose: Sign of 2D cross product of (a-b),(b-c) with epsilon
 * Info: proposed name SgnTurnXY (T2); TU tu_1BC050
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest+verdicts_B
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
int func_001BC278(qword a, qword b, qword c)
{
    float g = (GetX(a) - GetX(b)) * (GetY(b) - GetY(c)) - (GetY(a) - GetY(b)) * (GetX(b) - GetX(c));
    if (g < -D_002C0AB0) return -1;
    if (D_002C0AB0 < g) return 1;
    return 0;
}
