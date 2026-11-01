struct E { int n0; int oid; int n8; int id; int f10; int f14; };
struct G;
extern struct G D_003DD490;
int IlevelCur__Fv(void);
int CploFindSwObjects_15E650(struct G *pg, int kind, int oid, int a, int c, void **apv);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *pv, int n);
void *func_001F67E0(void *pv, int n);
void SeekSma(void *p, int id);
void SetSmaGoal(void *p, int id);
/** @sly2-annot addr=0x162250
 * Purpose: For one {state, oidRoot, oidSm, oidGoal, fOnLoadOnly, level} record matching the new state: find
 *   the SM's SMA and SeekSma/SetSmaGoal to oidGoal (default goal oids 0x612..0x615 for states 0..3)
 * Info: proposed name TriggerStateRsmg (T1); TU game
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void TriggerStateRsmg(void *pvUnused, struct E *pe, int n, int k)
{
    void *pv;
    void *ps;
    int id;
    if (pe->n0 != -1 && pe->n0 != n) return;
    if (pe->f10 && !k) return;
    if (pe->f14 != -1 && pe->f14 != IlevelCur__Fv()) return;
    if (CploFindSwObjects_15E650(&D_003DD490, 0x205, pe->oid, 0, 1, &pv) != 1) return;
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(pv, 0x30)) return;
    ps = func_001F67E0(pv, pe->n8);
    if (!ps) return;
    id = pe->id;
    if (pe->n0 == -1) {
        switch (n) {
        case 0: id = 0x612; break;
        case 1: id = 0x613; break;
        case 2: id = 0x614; break;
        case 3: id = 0x615; break;
        }
    }
    if (id != -1) {
        if (k == 1 || k == 4)
            SeekSma(ps, id);
        else
            SetSmaGoal(ps, id);
    }
}
