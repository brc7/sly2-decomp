struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[8]; };
struct X { char pad[0xC]; VT *pvt; char pad2[0x60 - 0x10]; X *p60; };
struct Ref;
extern "C" void func_00259BB0(Ref *, X *);
extern "C" void func_00259B48(Ref *, X *);
struct Ref {
    X *px;
    Ref(X *p) { func_00259BB0(this, p); }
    Ref(const Ref &r) { func_00259BB0(this, r.px); }
    Ref &operator=(const Ref &r) { func_00259B48(this, r.px); return *this; }
    ~Ref() { if (px) px->pvt->a[2].pfn((char *)px + px->pvt->a[2].d); }
};
struct S { int c; Ref a[8]; };
struct P { char pad[0x204]; S s; };
extern "C" X *func_001FCFF8(P *, int);
/** @sly2-annot addr=0x1FE478
 * Purpose: Ref to a music STREAMSND on the stack matching id
 * Info: proposed name FindMusic (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" Ref func_001FE478(P *p, int n)
{
    X *v = func_001FCFF8(p, n);
    int i;
    for (i = 0; i < p->s.c; i++)
        if (p->s.a[i].px->p60 == v) break;
    if (i == p->s.c) return Ref(0);
    Ref r(p->s.a[i]);
    p->s.c--;
    for (; i < p->s.c; i++) p->s.a[i] = p->s.a[i + 1];
    return r;
}
