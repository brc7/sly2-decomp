struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[3]; };
struct X { char pad[0xC]; struct VT *pvt; };
struct A { int a, b; };
struct E { char pad[0x54]; int c; struct A *ae; };
struct Y { char pad[0x60]; struct E *pe; };
struct Ref { struct Y *px; };
struct P { char pad[0x200]; struct Ref r; char pad2[0x22C - 0x204]; struct X *ap[6]; int n244; int n248; float g24C; };
void func_001FE0E8(struct P *, struct A *);
void SetSmaGoal(void *, int);
void StopSnd(struct Y *, float);
void func_00259B48(struct Ref *, void *);
void func_002333B4(void *, int, int);
static inline void Stop(struct Ref *pr)
{
    if (pr && pr->px) {
        StopSnd(pr->px, 0.0f);
        func_00259B48(pr, 0);
    }
}
/** @sly2-annot addr=0x1FE738
 * Purpose: Flush remaining dialog events, set role-5 SM to 0x611, stop stream, reset dialog state
 * Info: proposed name StopDialog (T1); TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void StopDialog(struct P *p)
{
    int i;
    struct E *pe;
    if (!p->r.px) return;
    pe = p->r.px->pe;
    while (p->n244 < pe->c) {
        func_001FE0E8(p, &pe->ae[p->n244]);
        p->n244++;
    }
    if (p->ap[5]) SetSmaGoal(p->ap[5], 0x611);
    Stop(&p->r);
    p->n244 = 0;
    p->n248 = 0;
    p->g24C = 0.0f;
    for (i = 0; i < 6; i++) {
        struct X *px = p->ap[i];
        if (px) px->pvt->a[2].pfn((char *)px + px->pvt->a[2].d);
    }
    func_002333B4(p->ap, 0, 0x18);
}
