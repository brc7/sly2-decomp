struct K { int a, b; };
struct S { char pad[0xE4]; int ne4; };
struct G;
extern struct G D_003DD490;
extern struct K D_002A77C0;
void OnLoAdd(struct S *p);
void EnsureSwCallback(struct G *pg, struct S *p, struct K k, int n);
/** @sly2-annot addr=0x1CA8E0
 * Purpose: PROXY vt23: LO::Unfreeze then 0x149AB8
 * Info: proposed name OnProxyAdd (T1); class PROXY slot23; TU proxy
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation) (was PROXY_Unfreeze)
 * Confidence: high [T1 0.85] - evidence: arbitration; sources: arbitration_slots22_23+verdicts_tu_r3r5
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
void OnProxyAdd(struct S *p)
{
    OnLoAdd(p);
    if (!p->ne4) EnsureSwCallback(&D_003DD490, p, D_002A77C0, 0);
}
