extern void *D_002DE9F0[];
/** @sly2-annot addr=0x15F888
 * Purpose: Stores the font in the font table D_002DE9F0[font->id]
 * Info: proposed name RegisterFont (T2); TU font
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
void func_0015F888(int *p)
{
    D_002DE9F0[*p] = p;
}
