typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
/** @sly2-annot addr=0x173588
 * Purpose: True if a projected point is in front (z>1e-4) and within +/-r in x and y
 * Info: proposed name FInScreenRect (T2); TU tu_173158
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
int func_00173588(qword v, float r) {
    int f = 0;
    if (GetZ(v) > 0.0001f) {
        float x = GetX(v);
        if (-r < x && x < r) {
            float y = GetY(v);
            if (-r < y && y < r) f = 1;
        }
    }
    return f;
}
