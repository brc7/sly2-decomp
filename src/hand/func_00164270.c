typedef struct CRef { int m_tagk; int m_tag; } CRef;
struct Y;
struct X { char pad[0x54]; int n; char pad1[0x64 - 0x58]; struct X *f64; char pad2[0x7C - 0x68]; struct X *f7c; char pad3[0x94 - 0x80]; int c; struct X **apx; char pad4[0xE0 - 0x9C]; int fe0; int pad5; int fe8; };
extern char D_003D4A60[];
void func_002581C0(CRef *pref, void *a, void *b);
void func_00129EC8(CRef *pref);
void func_001657D0(struct X *p, int n);
/** @sly2-annot addr=0x164270
 * Purpose: Reads the task's saved state and flag vars and sets the derived state (SetTaskState), handling
 *   start tasks of jobs
 * Info: proposed name RestoreTaskState (T1); TU game
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/engine/game-progression.md#devtools-findings-v3-2026-11-01,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00164270(struct X *p)
{
    CRef ref;
    int *pn;
    int n = 0;
    int f;
    pn = &p->fe0;
    f = 0;
    if (*pn) {
        int nT;
        func_002581C0(&ref, pn, D_003D4A60);
        nT = ref.m_tag;
        func_00129EC8(&ref);
        n = nT;
    }
    pn = &p->fe8;
    if (*pn) {
        int nT;
        func_002581C0(&ref, pn, D_003D4A60);
        nT = ref.m_tag;
        func_00129EC8(&ref);
        f = nT;
    }
    if (f && p->f7c && n < 3 && n > 0) {
        struct X *pxOwner = p->f7c;
        int i;
        p->f64 = 0;
        n = 1;
        if (pxOwner->f64 == p)
            pxOwner->f7c = 0;
        for (i = 0; i < p->c; i++) {
            if (p->apx[i]->n < 2) {
                n = 0;
                break;
            }
        }
    }
    if (!n)
        n = p->c == 0;
    func_001657D0(p, n);
}
