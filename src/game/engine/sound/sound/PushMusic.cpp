struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[8]; };
struct X { char pad[0xC]; VT *pvt; };
struct Ref;
extern "C" void func_00259BB0(Ref *, X *);
extern "C" void func_00259B48(Ref *, X *);
struct Ref {
    X *px;
    Ref(X *p) { func_00259BB0(this, p); }
    Ref(const Ref &r) { func_00259BB0(this, r.px); }
    Ref &operator=(const Ref &r) { func_00259B48(this, r.px); return *this; }
    Ref &operator=(X *p) { func_00259B48(this, p); return *this; }
    ~Ref() { if (px) px->pvt->a[2].pfn((char *)px + px->pvt->a[2].d); }
};
struct S { int c; Ref a[8]; };
struct P { char pad[0x204]; S s; };
extern int D_002DD5BC;
extern int D_002F7900;
extern int D_002E1E40;
extern "C" {
Ref func_001FE478(P *, int);
void StopSnd(X *, float);
X *func_001FCFF8(P *, int);
X *PstreamsndNew(X *, int);
void func_001FC408(X *, int, int, float, float *);
}
struct V2 { float x, y; };
static inline void Set(V2 *pv, float x, float y) { pv->x = x; pv->y = y; }
/** @sly2-annot addr=0x1FE150
 * Purpose: Start music id: stop current top, reuse or create STREAMSND kind 4 with player-object sources,
 *   push on 8-deep music stack
 * Info: proposed name PushMusic (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" X *PushMusic(P *p, int n)
{
    Ref r = func_001FE478(p, n);
    X *top = p->s.c > 0 ? (p->s.a + p->s.c - 1)->px : 0;
    if (top) StopSnd(top, 0.0f);
    if (r.px) {
        r.px->pvt->a[7].pfn((char *)r.px + r.px->pvt->a[7].d);
    } else {
        X *v = func_001FCFF8(p, n);
        if (!v || p->s.c == 8) return 0;
        float f = 1.0f;
        r = PstreamsndNew(v, 4);
        V2 af;
        Set(&af, 2000.0f, 5000.0f);
        func_001FC408(r.px, 3, D_002DD5BC, f, &af.x);
        func_001FC408(r.px, 3, D_002F7900, f, &af.x);
        func_001FC408(r.px, 4, D_002E1E40, f, &af.x);
    }
    return (p->s.a[p->s.c++] = r).px;
}
