struct G;
extern struct G D_003DD490;
struct O { char pad[0xF38]; void *p; };
void HandleBasicMessage(void *p, int msg, void *pm);
struct O *PpoHero(struct G *pg);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *o, int n);
void RefreshReconPendingTargets(void *p);
void UpdateReconChecklist(void *p);
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
