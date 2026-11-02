struct E { char pad[0x14]; int id; };
struct S { unsigned short c; short pad; struct E *ae; };
/** @sly2-annot addr=0x2142B0
 * Purpose: id in sprite anim list
 * Info: proposed name FSpriteHasAnim (T1); TU hud
 * Notes: names_v2 v1 proposed FUN_001ae130 (0.000)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
int func_002142B0(void *pv, struct S *p, int id)
{
    int i;
    if (id == -1)
        return 0;
    for (i = 0; i < p->c; i++) {
        if (id == p->ae[i].id)
            return 1;
    }
    return 0;
}
