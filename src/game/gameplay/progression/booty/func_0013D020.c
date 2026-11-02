struct I { int x0; int x4; int x8; int xC; int x10; int x14; };
struct A { char pad[0x158]; struct I *pi; void *p15C; void *p160; };
struct Q { char pad[0x11A8]; struct A *pa; };
struct P { char pad[0x188]; int x188; int x18C; int x190; struct A *pa; };
extern int D_003E0F9C;
extern char D_003DA0C0[];
extern char D_002DEB30[];
int FIsBasicDerivedFrom__FP5BASIC3CID(struct Q *q, int k);
void PostNotice(void *pv, int n, float g);
void *PsndStartSound(int n);
void SetSndSourceLo(void *pv, int n);
void BroadcastGameEvent(void *pv, int n, struct P *p);
void UpdateStepplayerBootySave(struct Q *q, int n, struct P *p);
void SetBootyState(struct A *pa, int n);
void func_00184130(struct P *p, struct Q *q);
/** @sly2-annot addr=0x13D020
 * Purpose: cid 0x25 vt51: when a STEPPLAYER touches: refuse (message 0x124, sound 0x36) if locked, broadcast
 *   event 0x32 on mismatch, else swap booty between player and container (UpdateStepplayerBootySave case 0)
 *   and sound 0x27
 * Info: proposed name OnBootyContainerTouched (T2); class CID37 slot51; TU booty
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void func_0013D020(struct P *p, struct Q *q)
{
    struct A *pqa, *ppa;
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x34))
        return;
    pqa = q->pa;
    ppa = p->pa;
    if (!pqa && !ppa)
        return;
    if (ppa && D_003E0F9C && !ppa->pi->x10) {
        PostNotice(D_003DA0C0, 0x124, 15.0f);
        SetSndSourceLo(PsndStartSound(0x36), 0);
        return;
    }
    if (p->x190 || (pqa && pqa->pi->x14 != p->x188)) {
        BroadcastGameEvent(D_002DEB30, 0x32, p);
        return;
    }
    if (p->pa)
        SetSndSourceLo(PsndStartSound(0x27), 0);
    UpdateStepplayerBootySave(q, 0, p);
    if (q->pa && p->pa) {
        q->pa->p15C = p;
        SetBootyState(q->pa, 3);
        p->pa->p160 = q;
        SetBootyState(p->pa, 2);
    } else if (q->pa) {
        q->pa->p15C = p;
        SetBootyState(q->pa, 3);
    } else {
        p->pa->p160 = q;
        SetBootyState(p->pa, 2);
    }
    func_00184130(p, q);
}
