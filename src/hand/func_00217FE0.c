struct E { char pad[8]; short h; char pad2[14]; };
struct F { char pad[8]; float f; char pad2[12]; };
struct S { int a; struct F *af; struct E *ae; };
void func_00218060(struct S *p, int i, int j, float f);
/** @sly2-annot addr=0x217FE0
 * Purpose: clamp g to [0,1], seek anim to g*duration
 * Info: proposed name SetSpriteAnimPhase (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00217FE0(struct S *p, int i, int j, float f)
{
    int h;
    float c = 0.0f;
    float one = 1.0f;
    c = (f > c) ? f : c;
    c = (c < one) ? c : one;
    h = p->ae[i].h;
    if (h >= 0)
        func_00218060(p, i, j, p->af[h].f * c);
}
