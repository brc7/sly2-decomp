typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0, %1" : "=r"(y) : "r"(q)); return y; }
/** @sly2-annot addr=0x1BC208
 * Purpose: Lexicographic x then y compare
 * Info: proposed name NCmpPosXY (T2); TU tu_1BC050
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
int func_001BC208(qword a, qword b)
{
    if (GetX(a) < GetX(b)) return -1;
    if (GetX(b) < GetX(a)) return 1;
    if (GetY(a) < GetY(b)) return -1;
    if (GetY(b) < GetY(a)) return 1;
    return 0;
}
