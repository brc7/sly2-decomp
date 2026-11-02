struct S { char pad[0xF04]; int n; };
extern char D_002DE2E0[];
int func_0014FD50(void *p);
/** @sly2-annot addr=0x18D500
 * Purpose: JT vt106: 0 if jts==1 or 0x14FD50(D_002DE2E0) else 1
 * Info: proposed name JT_vt106 (T2); class JT slot106; TU jt
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
int func_0018D500(struct S *p)
{
    int r = 1;
    if (p->n == r) return 0;
    if (func_0014FD50(D_002DE2E0)) r = 0;
    return r;
}
