typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
void InvertMatrix4Rigid(M4 *pm);
/** @sly2-annot addr=0x1A72D0
 * Purpose: Convert point through explicit M4 from/to matrices (to inverted)
 * Info: proposed name ConvertMatPos (T2); TU lo
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
qword func_001A72D0(M4 *pm, int f, qword v)
{
    if (pm) {
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
                : "=j"(v) : "j"(v), "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(pm->r[3]));
    }
    if (f) {
        M4 m;
        InvertMatrix4Rigid(&m);
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
                : "=j"(v) : "j"(v), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(m.r[3]));
    }
    return v;
}
