struct S {
    char pad[0x40]; int x40;
    char pad2[0xE0 - 0x44]; int xE0; int xE4;
    char pad3[0x118 - 0xE8]; int x118;
    char pad4[0x154 - 0x11C]; float x154; int x158; int x15c, x160; float x164; float x168;
};
void InitXfm(struct S *s);
void SetLoBusyk(struct S *s, int n);
/** @sly2-annot addr=0x1A3360
 * Purpose: LIGHT vt7 Init: InitXfm, kind 0, slot -1, falloff 200/2000, angle 240, ...
 * Info: proposed name InitLight (T1); class LIGHT slot7; TU light
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3360(struct S *s)
{
    int v;
    InitXfm(s);
    s->xE0 = 0;
    s->xE4 = -1; s->x118 = -1;
    s->x164 = 200.0f;
    s->x168 = 2000.0f;
    s->x154 = 240.0f;
    s->x158 = 1;
    SetLoBusyk(s, 1);
    v = s->x40;
    if ((v ^ 1) & 1) *(int *)v &= ~0x20;
    else s->x40 = v & ~0x20;
}
