typedef struct { int n; int a; } ENT;
typedef struct {
    char pad[0x48];
    int c;
    ENT ae[64];
    int i;
    int first;
    int x254;
    int x258;
} OBJ;
extern int D_003DA570;
extern int D_002C35B4;
int func_001DBBA0(OBJ *);
void func_001DA9E8(OBJ *);
static inline int Max(int a, int b) { return a < b ? b : a; }
static inline int Clamp(int n, int lo, int hi)
{
    if (n < lo)
        return lo;
    if (n > hi)
        return hi;
    return n;
}
/** @sly2-annot addr=0x1DBBE0
 * Purpose: Select row i (clamped), scroll the window, set current power-up/loot id
 * Info: proposed name SelectThiefnetItem (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SelectThiefnetItem(OBJ *p, int i)
{
    if (p->c == 0) {
        p->first = 0;
        p->i = -1;
        p->x254 = -1;
        p->x258 = -1;
    } else {
        i = Clamp(i, 0, p->c - 1);
        p->i = i;
        if (D_003DA570) {
            if (i < p->first) {
                p->first = i;
            } else {
                while (func_001DBBA0(p) - 1 < p->i)
                    p->first++;
            }
            if (p->first == 1)
                p->first = 0;
        } else {
            int n = Clamp(p->first, Max(i - D_002C35B4 + 1, 0), i);
            p->first = n;
        }
        p->x254 = p->ae[p->i].n;
        p->x258 = p->ae[p->i].n;
    }
    func_001DA9E8(p);
}
