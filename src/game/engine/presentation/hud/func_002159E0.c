struct S { int a; int k; };
void func_00216BC8(struct S *p, int a, int b, int c);
void func_002174C8(struct S *p, int a, int b, int c);
void func_002177E0(struct S *p, int a, int b, int c);
/** @sly2-annot addr=0x2159E0
 * Purpose: dispatch by kind: 3 text (0x216BC8), 1 geometry (0x2174C8), else picture (0x2177E0)
 * Info: proposed name RenderSpritePayload (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_002159E0(struct S *p, int a, int b, int c)
{
    switch (p->k) {
    case 3:
        func_00216BC8(p, a, b, c);
        break;
    case 1:
        func_002174C8(p, a, b, c);
        break;
    case 6:
    case 7:
        func_002177E0(p, a, b, c);
        break;
    case 2:
    case 4:
    case 5:
    case 8:
        break;
    }
}
