struct G;
extern struct G D_003DD490;
struct O { char pad[0xF38]; void *p; };
void HandleBasicMessage(void *p, int msg, void *pm);
struct O *PpoHero(struct G *pg);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *o, int n);
void RefreshReconPendingTargets(void *p);
void UpdateReconChecklist(void *p);
/** @sly2-annot addr=0x203670
 * Purpose: CID 0x2E HandleMessage: on msg 0x11 refresh player recon list and checklist
 * Info: proposed name HandleReconTargetMessage (T1); class CID46 slot5; TU tu_201D78
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void HandleReconTargetMessage(void *p, int msg, void *pm)
{
    struct O *o;
    HandleBasicMessage(p, msg, pm);
    if (msg != 0x11) return;
    o = PpoHero(&D_003DD490);
    if (o && FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x34) && o->p) {
        RefreshReconPendingTargets(o->p);
        UpdateReconChecklist(o->p);
    }
}
