typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE2A0(MSG *pmsg);
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE2A0(pmsg); }
struct C { int a[4]; };
struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; int pad[6]; unsigned long fl; };
struct DA { int n0; int cMax; int c; struct O **a; };
struct S { char pad[0xE0]; int c; struct O **apo; };
void func_001AE478(struct C *);
void func_001AE4A0(struct C *, int);
void func_00231170(struct S *p, void *q);
void func_0013F888(struct DA *pda);
void func_002317E0(struct S *p, void *q, struct DA *pda);
int func_00132B10(void *p, int n);
/** @sly2-annot addr=0x15C1E8
 * Purpose: EXPLG::LoadFromBrx: XFM load, reads child explosion list into apo
 * Info: proposed name LoadExplgFromBrx (T1); class EXPLG slot14; TU emitter
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8+parse-verified
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void func_0015C1E8(struct S *p, void *q)
{
    struct C ctx;
    struct DA da;
    MSG msg;
    int i;
    func_00231170(p, q);
    func_001AE478(&ctx);
    func_0013F888(&da);
    func_002317E0(p, q, &da);
    p->apo = PvAllocMsg(&msg, da.c * 4, 0x10);
    for (i = 0; i < da.c; i++) {
        struct O *o = da.a[i];
        o->fl |= 0x200000000000000L;
        if (!func_00132B10(o, 0x1E)) {
            struct VTE *e = &o->vt[9];
            e->pfn((char *)o + e->d);
        } else {
            p->apo[p->c++] = o;
        }
    }
    func_001AE4A0(&ctx, 2);
}
