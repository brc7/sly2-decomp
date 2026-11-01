typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct Pr { qword a; qword b; };
struct Q2 { qword pad; qword v; char pad2[0x20]; };
struct S2 { struct Pr ap[2]; qword v40; struct Q2 *aq; };
struct E { char pad[0xA0]; int nA0; char pad2[0x1C]; int nC0; };
struct P { char pad[0xE8]; struct E *pe; };
qword func_001FAA00(struct P *p, qword v);
qword func_001FAA48(struct P *p, qword a, qword v);
/** @sly2-annot addr=0x1FAD00
 * Purpose: Applies an impulse derived from a contact record (v40) when SO has dynamics data
 * Info: proposed name ApplySoContactImpulse (T2); class SO slot68; TU solve
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001FAD00(struct P *p, int a1, struct S2 *ps, int a3, int k)
{
    struct E *pe = p->pe;
    qword v;
    struct Pr *pr;
    if (!pe) return;
    v = ps->v40;
    if (k) {
        U u;
        __asm__("qmtc2.ni %1, %0" : "=j"(u.q) : "r"(-1.0f));
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(v) : "j"(v), "j"(u.q));
    }
    pr = &ps->ap[k];
    if (pe->nC0 != 3) pr->a = func_001FAA00(p, v);
    if (pe->nA0 != 3) pr->b = func_001FAA48(p, ps->aq[k].v, v);
}
