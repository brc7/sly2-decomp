struct S { char pad[0x2044]; void *p2044; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern int D_003E1BEC;
void func_0014D1D0(struct CLOCK *pclock, int n, float g);
void func_001FB098(void *p, float g);
void func_00258040(void **pp, int n);
void StopStepplayerGadget(struct S *p, int n);
/** @sly2-annot addr=0x18DD78
 * Purpose: JT vt102: for gadget 0x1D/0x2A restore clock rate 1.0, stop sound x2044; else base 0x141E28
 * Info: proposed name EndJtGadget (T2); class JT slot102; TU jt
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/player.md#5-sly-jt-jtcpp-0x188488--component-tus,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018DD78(struct S *p, int n)
{
    if (n == 0x1D || n == 0x2A) {
        void **pp;
        func_0014D1D0(&D_002DDED0, 1, 1.0f);
        pp = &p->p2044;
        if (*pp) {
            func_001FB098(*pp, 0.0f);
            func_00258040(pp, 0);
        }
        D_003E1BEC = 0;
    } else {
        StopStepplayerGadget(p, n);
    }
}
