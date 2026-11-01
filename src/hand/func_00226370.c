typedef int qword __attribute__((mode(TI)));
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
struct S { char pad[0x5C]; int x5C; char pad2[0x18C - 0x60]; float f18C; float f190; char pad3[0x1CC - 0x194]; int k; char pad4[8]; int h; };
int func_0012DC18(struct S *p, int c, int a, int b, int d, qword q);
void func_0012E460(int *ph);
/** @sly2-annot addr=0x226370
 * Purpose: state change; on enable builds a target vector and calls 0x12DC18/0x12E460
 * Info: proposed name SetTurretActiveState (T2); TU turret
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
void func_00226370(struct S *p, int k)
{
    if (p->k != k) {
        p->k = k;
        if (k) {
            union { qword q; float f[4]; } v;
            v.q = Make(0.0f, -p->f190, p->f18C);
            p->h = func_0012DC18(p, p->x5C, 1, 5, 3, v.q);
        } else {
            func_0012E460(&p->h);
        }
    }
}
