struct G;
extern struct G D_003DD490;
struct O { char pad[0xF38]; void *p; };
void func_00133590(void *p, int msg, void *pm);
struct O *func_001C4840(struct G *pg);
int func_00132B10(void *o, int n);
void func_00202030(void *p);
void func_002021F8(void *p);
/** @sly2-annot addr=0x203670
 * Purpose: CID 0x2E HandleMessage: on msg 0x11 refresh player recon list and checklist
 * Info: proposed name HandleReconTargetMessage (T1); class CID46 slot5; TU tu_201D78
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void func_00203670(void *p, int msg, void *pm)
{
    struct O *o;
    func_00133590(p, msg, pm);
    if (msg != 0x11) return;
    o = func_001C4840(&D_003DD490);
    if (o && func_00132B10(o, 0x34) && o->p) {
        func_00202030(o->p);
        func_002021F8(o->p);
    }
}
