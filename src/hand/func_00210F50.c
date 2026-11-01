extern struct G { char pad[0x4764]; int a; int b; } D_003DD490;
extern char D_002A8AA8[];
void fiprintf(void *f, const char *fmt, ...);
/** @sly2-annot addr=0x210F50
 * Purpose: STRINGGEN id 0x2D0: sprintf("%d/%d", SW+0x4764, SW+0x4768) = clue bottles collected/total
 * Info: proposed name BuildClueCountText (T1); TU hud
 * Notes: names_v2 v1 proposed AddButtonOffAseg__FP6BUTTON3OID (0.000) | runtime: SW+0x4764/+0x4768 are clue
 *   collected/total counters
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r7c_hud+runtime-corrected
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00210F50(void *f)
{
    fiprintf(f, D_002A8AA8, D_003DD490.a, D_003DD490.b);
}
