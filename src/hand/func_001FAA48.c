typedef int qword __attribute__((mode(TI)));
qword func_001A62E8(void *p);
qword func_001FAA98(void *p, qword q);
/** @sly2-annot addr=0x1FAA48
 * Purpose: Angular velocity change: (pos - PosLoWorld) x impulse through inverse inertia (0x1FAA98)
 * Info: proposed name DwFromSoImpulse (T2); TU solve
 * Notes: names_v2 v1 proposed FAbsorbSoWkr__FP2SOP3WKR (0.001)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
qword func_001FAA48(void *p, qword a, qword b)
{
    qword r, c, d;
    d = func_001A62E8(p);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(d));
    c=b;
    __asm__("vopmula.xyz $ACC, %1, %0\n\tvopmsub.xyz %0, %0, %1" : "+j"(c) : "j"(r));
    return func_001FAA98(p, c);
}

typedef char angular_input_layout[(sizeof(qword)==16 && __alignof__(qword)==16 && sizeof(void*)==4)?1:-1];
