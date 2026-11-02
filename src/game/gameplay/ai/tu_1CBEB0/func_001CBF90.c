struct S { char pad[0x1010]; int n; };
struct G { char pad[0x600]; int a; };
extern struct G D_002DDF40;
extern int D_002F7900;
void SetCmPolicy(struct G *pg, int n, void *pv, int m, void *p);
void RevokeCmPolicy(struct G *pg, int a, int b, void *c, void *d, int e);
/** @sly2-annot addr=0x1CBF90
 * Purpose: Toggle camera policy 6 framing g_pmurray (D_002F7900) and this guard (SetCmPolicy/RevokeCmPolicy)
 * Info: proposed name SetRajanCameraFocus (T2); class RAJAN slot90; TU tu_1CBEB0
 * Notes: class identity from whole_game WE names/object strings: SetCid61GuardCameraFocus ->
 *   SetRajanCameraFocus
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest+whole_game(class identity)
 * See: docs/engine/bosses.md#32-rajan-m3_boss-l12-temple-exterior,
 *   docs/reference/functions/guards-ai.md#tu_1cbeb0-0x1cbeb0-0x1cc158
 * @end-sly2-annot */
void func_001CBF90(struct S *p, int n)
{
    if (p->n == n) return;
    p->n = n;
    if (n) SetCmPolicy(&D_002DDF40, 6, &D_002DDF40.a, D_002F7900, p);
    else RevokeCmPolicy(&D_002DDF40, 1, 6, 0, 0, 0);
}
