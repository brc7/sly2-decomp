typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
M3 *func_001A6318(void *p);
void InvertMatrix3Cofactor(M3 *pm, M3 *pn);
/** @sly2-annot addr=0x1A6FC0
 * Purpose: Same for direction vectors (rotation only, M3)
 * Info: proposed name ConvertAloVec (T1); TU lo
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/object-model.md#9-key-functions, docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
qword ConvertAloVec(void *a, void *b, qword v)
{
    if (a) {
        M3 *pm = func_001A6318(a);
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "j"(v), "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]));
    }
    if (b) {
        M3 m;
        InvertMatrix3Cofactor(&m, func_001A6318(b));
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "j"(v), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    }
    return v;
}
