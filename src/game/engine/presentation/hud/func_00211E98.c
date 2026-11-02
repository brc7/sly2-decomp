struct E { int a; int b; };
struct S { char pad[0x194]; unsigned int c; struct E ae[16]; };
/** @sly2-annot addr=0x211E98
 * Purpose: CID 0x81: append oid to list B (+0x194, max 16)
 * Info: proposed name AddWidget129OidB (T2); TU hud
 * Notes: names_v2 v1 proposed SetButtonRsmg__FP6BUTTONi3OIDN22 (0.000) | v2: demoted to T2 (placeholder token
 *   in name: 129)
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00211E98(struct S *p, int v)
{
    unsigned int c = p->c;
    if (c < 16) {
        p->ae[c].a = v;
        p->c = c + 1;
    }
}
