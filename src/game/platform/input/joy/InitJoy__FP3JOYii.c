struct S { char pad[0x1000]; int a; int b; char p1[0xE0]; int n; float g; };
void SetJoyJoys__FP3JOY4JOYS4JOYK(struct S *p, int a, int b);
void scePadPortOpen(int a, int b, struct S *p);
/** @sly2-annot addr=0x187150
 * Purpose: SetJoyJoys(searching), store port/slot, scePadPortOpen, enable rumble, gain 1.0
 * Info: proposed name InitJoy (T1); TU joy
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#joy-0x187030-0x188488
 * @end-sly2-annot */
void InitJoy__FP3JOYii(struct S *p, int a, int b)
{
    SetJoyJoys__FP3JOY4JOYS4JOYK(p, 1, 0);
    p->a = a;
    p->b = b;
    scePadPortOpen(a, b, p);
    p->n = 1;
    p->g = 1.0f;
}
