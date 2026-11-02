struct S { char pad[0x28]; int f; };
void func_00218060(struct S *p, int a, int b);
/** @sly2-annot addr=0x218038
 * Purpose: time seek of default anim
 * Info: proposed name SetSpriteDefaultTime (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00218038(struct S *p)
{
    if (p->f)
        func_00218060(p, 0, 0);
}
