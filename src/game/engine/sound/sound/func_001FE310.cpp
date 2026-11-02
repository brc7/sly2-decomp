struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[8]; };
struct X { char pad[0xC]; VT *pvt; char pad2[0x40 - 0x10]; int n40; char pad3[0x50 - 0x44]; unsigned short fl; unsigned short c52; };
struct S { int c; void *a[8]; };
struct P { char pad[0x204]; S s; };
struct Ref {
    X *px;
    ~Ref() { if (px) px->pvt->a[2].pfn((char *)px + px->pvt->a[2].d); }
};
extern "C" void func_001FE478(Ref *, P *, int);
extern "C" void func_001FB098(X *, float);
extern "C" void func_00259B48(Ref *, void *);
/** @sly2-annot addr=0x1FE310
 * Purpose: Stop music id if playing (0x1FE478 lookup), StopSnd
 * Info: proposed name StopMusic (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void func_001FE310(P *p, int n)
{
    Ref r;
    func_001FE478(&r, p, n);
    if (r.px) {
        int f = 0;
        if (!r.px->n40) { f = r.px->c52 || (r.px->fl & 1); f ^= 1; }
        if (!f) {
            func_001FB098(r.px, 0.0f);
            func_00259B48(&r, p->s.c > 0 ? *(p->s.a + p->s.c - 1) : 0);
            if (r.px) r.px->pvt->a[7].pfn((char *)r.px + r.px->pvt->a[7].d);
        }
    }
}
