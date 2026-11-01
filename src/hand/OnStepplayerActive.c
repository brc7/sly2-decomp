struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x1310 - 0x10]; char sub[4]; int n1314; };
void OnPoActive(void *p, int a);
void ReleaseAsega(void *p);
/** @sly2-annot addr=0x144480
 * Purpose: vt78: PO OnPoActive, then vt105; when deactivated stop +0x1310 action and +0x1314=-1
 * Info: proposed name OnStepplayerActive (T1); class STEPPLAYER slot78; TU stepplayer
 * Notes: runtime4: OnStepplayerActive(0) clears the gadget overlay on character switch (code path) | mangled
 *   dropped (base does not match final name): OnActive__10STEPPLAYERi
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r2b_rest+runtime
 * See: docs/engine/player.md#6-murray-murraycpp-0x1b4728, docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void OnStepplayerActive(struct O *p, int a)
{
    struct VTE *e;
    OnPoActive(p, a);
    e = &p->vt[105];
    ((void (*)(void *))e->pfn)((char *)p + e->d);
    if (!a) {
        ReleaseAsega(p->sub);
        p->n1314 = -1;
    }
}
