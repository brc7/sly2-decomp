typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
void func_0011B340(M3 *pm);
/** @sly2-annot addr=0x1A7348
 * Purpose: Same for vectors with M3
 * Info: proposed name ConvertMatVec (T2); TU lo
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
qword func_001A7348(M3 *pm, int f, qword v)
{
    if (pm) {
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "j"(v), "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]));
    }
    if (f) {
        M3 m;
        func_0011B340(&m);
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "j"(v), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    }
    return v;
}
