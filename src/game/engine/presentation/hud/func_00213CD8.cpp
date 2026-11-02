struct M { int a[8]; } __attribute__((aligned(16)));
struct H { virtual void *Alloc(int cb, int k); };
struct E { char pad[0x14]; int id; };
struct A { unsigned short c; short pad; E *ae; char pad8[0x24]; M *x2C; };
struct S { char pad[0x80]; unsigned short c80; short pad82; A *aa; char pad88[0x3C]; H *xC4; };
/** @sly2-annot addr=0x213CD8
 * Purpose: set M23 transform of the sprite that holds anim id
 * Info: proposed name SetWidgetAnimXf (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void func_00213CD8(S *p, int k, M *pm)
{
    int j, l;
    for (j = 0; j < p->c80; j++) {
        A *a = &p->aa[j];
        for (l = 0; l < a->c; l++) {
            E *e = &a->ae[l];
            if (e->id == k) {
                if (!a->x2C)
                    a->x2C = (M *)p->xC4->Alloc(0x20, 0x1F);
                *a->x2C = *pm;
                return;
            }
        }
    }
}
