typedef unsigned long ulong;
struct X { char pad[0x40]; int n40; char pad2[0x4E - 0x44]; short n4E; unsigned short f50; unsigned short n52; char pad3[0x60 - 0x54]; struct S *ps; char pad4[0x68 - 0x64]; ulong ul68; };
struct Ent { short h0; short pad; int n4; };
struct S { char pad[0x50]; float f50; int n54; struct Ent *a58; };
struct P { char pad[0x200]; struct X *r200; char pad2[0x244 - 0x204]; int n244; float f248; float f24C; };
extern float D_002DDF10;
void StopDialog(struct P *);
void func_001FE0E8(struct P *, struct Ent *);
/** @sly2-annot addr=0x1FDEC8
 * Purpose: Advance current dialog: by elapsed stream time fire lip-sync/anim events (0x1FE0E8); end ->
 *   StopDialog
 * Info: proposed name UpdateDialog (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FDEC8(struct P *p)
{
    struct X *x = p->r200;
    struct S *ps;
    int f;
    if (!x) return;
    {
        int g = x->n52 || (x->f50 & 1);
        if (g) return;
    }
    x = p->r200;
    if (x->n4E > 0) return;
    ps = x->ps;
    p->f24C += D_002DDF10;
    f = 0;
    if (!x->n40) {
        f = x->n52 || (x->f50 & 1);
        f ^= 1;
    }
    if (f) {
        if (0.0f < (float)p->r200->ul68 / 294912000.0f || ps->f50 <= p->f24C) StopDialog(p);
        return;
    }
    while (p->n244 < ps->n54) {
        struct Ent *pe = &ps->a58[p->n244];
        float d = (float)pe->h0 * 0.016666668f;
        if ((float)p->r200->ul68 / 294912000.0f < p->f248 + d) return;
        func_001FE0E8(p, pe);
        p->f248 += d;
        p->n244++;
    }
}
