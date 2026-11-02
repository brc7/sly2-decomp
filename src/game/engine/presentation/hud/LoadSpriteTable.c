struct S { char pad[0xC4]; void *xC4; };
extern int D_002C40A4;
int U16Read__18CBinaryInputStream(void *a);
int PvAllocClear__FP4HEAPii(void *heap, int cb, int k);
void LoadSpriteDescriptor(struct S *p, void *a, void *b, int n, int m);
/** @sly2-annot addr=0x214398
 * Purpose: read sprite count, alloc (n+1)*0x40, read each descriptor
 * Info: proposed name LoadSpriteTable (T1); TU hud
 * Notes: names_v2 v1 proposed FUN_001ae510 (0.000) | level_parser CONFIRMED-parse: payload layout followed by
 *   the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r7c_hud+parse-verified
 * See: docs/engine/hud-ui.md#5-key-functions,
 *   docs/formats/level-objects.md#8-widget-payload-loadwidgetfrombrx-0x212298-cparse,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void LoadSpriteTable(struct S *p, void *a, void *b, int *pn, int *pm)
{
    int i;
    *pn = U16Read__18CBinaryInputStream(a);
    *pm = PvAllocClear__FP4HEAPii(p->xC4, (*pn + 1) << 6, D_002C40A4 ? 0x13 : 0x11);
    for (i = 1; i <= *pn; i++)
        LoadSpriteDescriptor(p, a, b, *pn, *pm);
}
