typedef int qword __attribute__((mode(TI)));
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
struct S { char pad[0x94]; int k; };
qword func_0011BFF0(float *m);
/** @sly2-annot addr=0x22CA18
 * Purpose: Extract an XF component value from a matrix by component kind (+0x94): translation row
 *   (1,4,5,9,10), Euler angles (6,7), shear (11), scale diagonal (12); kinds 0/15 nothing
 * Info: proposed name QwXfValueFromMat (T2); TU xf
 * Notes: descriptive only: xf: build matrix/vector from kind | no usable identifier; purpose only |
 *   top50_round (0.50): Body switch on +0x94 with the same kind groups SlotSetRole uses when it captures the
 *   current value; callers UpdateAct, UpdateActXform, SlotCurrentMatrix, LoadSoXfsFromBrx, QwSlotValue.
 *   Sibling naming QwSlotValue 0x230CF0
 * Confidence: low (draft) [T2 0.50] - evidence: semantic review; sources: r8_21BB88+top50_round
 * See: docs/engine/animation.md#animation-format-summary-anim_lab-v31,
 *   docs/reference/functions/animation.md#act-0x12b3b0-0x12e600,
 *   docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
qword func_0022CA18(struct S *p, float *m)
{
    qword v;
    union { qword q; float f; } r;
    switch (p->k) {
    case 0: case 15: break;
    case 1: case 4: case 5: case 9: case 10:
        r.q = Make(m[12], m[13], m[14]); v = r.q; break;
    case 6: case 7:
        v = func_0011BFF0(m); break;
    case 11:
        r.q = Make(m[4], m[8], m[9]); v = r.q; break;
    case 12:
        r.q = Make(m[0], m[5], m[10]); v = r.q; break;
    }
    return v;
}
