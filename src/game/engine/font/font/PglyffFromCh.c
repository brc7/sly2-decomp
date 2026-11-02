typedef struct GLYPH { unsigned short ch; short a; short b; unsigned short dx; } GLYPH;
struct FONTBRX { char pad[0x138]; int cglyph; GLYPH *aglyph; };
extern unsigned short D_002BE338[];
/** @sly2-annot addr=0x15F8A8
 * Purpose: Binary-searches the font glyph table for a character (via remap table D_002BE338)
 * Info: proposed name PglyffFromCh (T1); TU font
 * Notes: names_v2 v1 proposed IglyffFromCh (0.976)
 * Confidence: high [T1 0.80] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
GLYPH *PglyffFromCh(struct FONTBRX *p, unsigned char ch)
{
    int key = D_002BE338[ch];
    int lo = 0;
    int hi = p->cglyph - 1;
    while (lo <= hi) {
        int mid = (hi + lo) / 2;
        int d = key - p->aglyph[mid].ch;
        if (d == 0)
            return &p->aglyph[mid];
        if (d > 0)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return 0;
}
