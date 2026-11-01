struct S { char pad[0x13C]; int x; };
void func_00221298(struct S *p, int a);
/** @sly2-annot addr=0x221268
 * Purpose: if changed store and call UpdateTnCallback(ptn,0)
 * Info: proposed name SetTnTnsOverride (T1); TU tn
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tn-0x220d40-0x223f70,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_00221268(struct S *p, int x)
{
    if (x != p->x) { p->x = x; func_00221298(p, 0); }
}
