struct S { char pad[0x7C]; int x7c; char pad2[0x90]; int x110; };
extern struct G D_003DD490;
void ShowWidget(struct S *p, int k);
void IncrementSwHandsOff(struct G *g, int a);
void PlayWidgetAnim(struct S *p, int a);
/** @sly2-annot addr=0x211CA8
 * Purpose: CID 0x7F slot49 override: if idle, ShowWidget + IncrementSwHandsOff + play anim 0x33A
 * Info: proposed name ShowWidget127 (T2); class WIDGET127 slot49; TU hud
 * Notes: names_v2 v1 proposed RenderBlots__Fv (0.043) | v2: demoted to T2 (placeholder token in name: 127)
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00211CA8(struct S *p, int k)
{
    if (p->x7c == -1) {
        ShowWidget(p, k);
        IncrementSwHandsOff(&D_003DD490, 0);
        p->x110 = 1;
        PlayWidgetAnim(p, 0x33A);
    }
}
