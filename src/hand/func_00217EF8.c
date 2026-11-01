struct E { char pad[8]; short h; char pad2[14]; };
struct S { short a; unsigned short c; char pad[4]; struct E *ae; };
/** @sly2-annot addr=0x217EF8
 * Purpose: first reuse slot with h<0
 * Info: proposed name IFreeSpriteSlot (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
int func_00217EF8(struct S *p)
{
    int i;
    int c = p->c;
    for (i = 0; i < c; i++) {
        if (p->ae[i].h < 0)
            return i;
    }
    return 0;
}
