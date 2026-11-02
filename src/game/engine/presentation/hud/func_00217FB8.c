struct S { char pad[0x28]; int f; };
void func_00217FE0(struct S *p, int a, int b);
/** @sly2-annot addr=0x217FB8
 * Purpose: phase seek of default anim
 * Info: proposed name SetSpriteDefaultPhase (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00217FB8(struct S *p)
{
    if (p->f)
        func_00217FE0(p, 0, 0);
}
