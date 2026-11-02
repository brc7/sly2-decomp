struct K { int a, b; };
extern struct K D_002A96F0;
extern char D_003DD490[];
struct S { char pad[0x13C]; int x; };
void UpdateLo(struct S *p);
void PostSwCallback(void *g, struct S *p, struct K k, int z);
/** @sly2-annot addr=0x221388
 * Purpose: UpdateLo then, if tnsOverride, PostSwCallback(g_sw, ptn, UpdateTnCallback)
 * Info: proposed name UpdateTn (T1); class TN slot16; TU tn
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void UpdateTn(struct S *p)
{
    UpdateLo(p);
    if (p->x) PostSwCallback(D_003DD490, p, D_002A96F0, 0);
}
