typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
struct G { char pad[0x3B58]; void *apA[9]; void *apB[9]; };
extern struct G D_003DD490;
extern char D_003D4A60[];
extern int D_002BE26C;
extern int D_002BE2D0[];
struct S { char pad[0x118]; int i; };
void func_002581C0(CRef *pref, void *p, void *q);
void func_00129EC8(CRef *pref);
/** @sly2-annot addr=0x15D798
 * Purpose: Number of menu items for the selected episode: table value minus 2 if never played (time 0), minus
 *   1 if under 100%
 * Info: proposed name CEpisodeMenuItems (T2); TU tu_15CD68
 * Notes: names_v2 v1 proposed BindExpls__FP5EXPLS (0.438)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/hud-ui.md#tu_15cd68-0x15cd68-0x15dc80
 * @end-sly2-annot */
int func_0015D798(struct S *p)
{
    void *pA, *pB;
    int n;
    CRef ref;
    float g;
    if (D_002BE26C)
        return 0x10;
    n = D_002BE2D0[p->i];
    pA = D_003DD490.apA[p->i];
    pB = D_003DD490.apB[p->i];
    func_002581C0(&ref, pA, D_003D4A60);
    g = ref.u.g;
    func_00129EC8(&ref);
    if (g == 0.0f) {
        n -= 2;
    } else {
        int c;
        func_002581C0(&ref, pB, D_003D4A60);
        c = ref.u.n;
        func_00129EC8(&ref);
        if (c <= 99)
            n--;
    }
    return n;
}
