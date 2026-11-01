struct E { char pad[8]; short h8; short pad2; float fC; int x10; void *x14; };
struct S { char pad[8]; struct E *ae; char padc[4]; unsigned long ul10; float f18; int x1C; int x20; };
extern struct CLOCK { unsigned long ticks; } D_002DDF18;
void func_001FB098(void *p, float f);
void func_00258040(void **pp, void *next);
void func_00217F38(struct S *p, int i);
static inline void Release(void **pp)
{
    if (pp && *pp) {
        func_001FB098(*pp, 0.0f);
        func_00258040(pp, 0);
    }
}
/** @sly2-annot addr=0x218660
 * Purpose: stop slot sounds (StopSnd) and reset its items
 * Info: proposed name StopSpriteSlot (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00218660(struct S *p, int i)
{
    struct E *e = (struct E*)(i*sizeof(struct E)+(int)p->ae);
    if (e->h8 >= 0) {
        float t=e->fC;
        p->x1C = e->h8;
        p->f18 = t;
        p->ul10 = D_002DDF18.ticks;
        p->x20 = -1;
        e->h8 = -1;
        e->x10 = 0;
        Release(&e->x14);
    }
    func_00217F38(p, i);
}
