typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
struct G { char pad[0x3B58]; void *apA[9]; void *apB[9]; };
extern struct G D_003DD490;
extern char D_003D4A60[];
extern char D_002DFC00[];
extern int D_002BE26C;
extern int D_002BE270;
struct S { char pad[0x110]; int i110; char pad2[4]; int i118; char pad3[0xC]; int n128; };
void func_002581C0(CRef *pref, void *p, void *q);
void func_00129EC8(CRef *pref);
void func_002123C8(struct S *p);
void func_0015D648(struct S *p, int a, int b);
void func_00187E78(void *p);
static inline float GFromP(void *pv)
{
    CRef ref;
    float g;
    func_002581C0(&ref, pv, D_003D4A60);
    g = ref.u.g;
    func_00129EC8(&ref);
    return g;
}
static inline int NFromP(void *pv)
{
    CRef ref;
    int n;
    func_002581C0(&ref, pv, D_003D4A60);
    n = ref.u.n;
    func_00129EC8(&ref);
    return n;
}
/** @sly2-annot addr=0x15CDA8
 * Purpose: WIDGET PostLoad (0x2123C8) then picks the first empty slot index from g_sw arrays
 * Info: proposed name PostWidget131Load (T2); class WIDGET131 slot15; TU tu_15CD68
 * Notes: Cid83->Widget131 (CID 0x83=131, vtable)
 * Confidence: low (draft) [T2 0.65] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/game-progression.md#6-objective-counters-job-markers-and-the-episode-menu,
 *   docs/reference/functions/hud-ui.md#tu_15cd68-0x15cd68-0x15dc80
 * @end-sly2-annot */
void func_0015CDA8(struct S *p)
{
    func_002123C8(p);
    p->i118 = 8;
    if (!D_002BE26C) {
        do {
            int n;
            void *pB;
            if (GFromP(D_003DD490.apA[p->i118]) != 0.0f)
                break;
            pB = D_003DD490.apB[p->i118 - 1];
            n = 0;
            if (pB)
                n = NFromP(pB);
            if (n >= 100)
                break;
            p->i118--;
            if (p->i118 < 2)
                break;
        } while (!D_002BE26C);
    }
    if (D_002BE270 == -1) {
        p->i110 = p->i118;
        func_0015D648(p, 0, -1);
    } else {
        p->n128 = D_002BE270;
        p->i110 = 0;
        D_002BE270 = -1;
        func_0015D648(p, 8, 0);
    }
    func_00187E78(D_002DFC00);
}
