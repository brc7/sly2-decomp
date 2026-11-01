struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
struct F { int a[0x18]; };
struct S { int n0; int n; float t; };
void *func_002333B4(void *pv, int c, int cb);
void func_001F7598(struct F *pf);
/** @sly2-annot addr=0x20D108
 * Purpose: state 2 -> InflictZap 0x4000
 * Info: proposed name SetStuckWatchState (T2); TU tu_20CE28
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/guards-ai.md#tu_20ce28-0x20ce28-0x20d5d0
 * @end-sly2-annot */
void func_0020D108(struct S *p, int n)
{
    if (p->n == n) return;
    p->n = n;
    p->t = D_002DDED0.t;
    if (n == 2) {
        struct F f;
        func_002333B4(&f, 0, sizeof f);
        f.a[3] = 0x4000;
        f.a[0] = p->n0;
        f.a[2] = p->n0;
        f.a[4] = 0;
        func_001F7598(&f);
    }
}
