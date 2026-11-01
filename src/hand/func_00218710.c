struct B { char pad[8]; short h; char pad2[6]; int p; };
struct S { char pad[8]; struct B *pb; };
/** @sly2-annot addr=0x218710
 * Purpose: h!=0 && !done
 * Info: proposed name FSpriteAnimActive (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
int func_00218710(struct S *p)
{
    struct B *b = p->pb;
    if (b->h != 0 && b->p == 0)
        return 1;
    return 0;
}
