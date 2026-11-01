struct R { int cb; int cbAlign; };
struct C { int a[4]; };
struct S { char pad[0x150]; int c; void *pv; };
struct C *func_001AE478(struct C *);
void *func_001AE378(struct R *);
void func_001F31E0(struct S *, void *);
void func_0013C440(struct S *);
void *func_001AE238(struct R *, void *);
void func_001AE4A0(struct C *, int);
static inline struct R *SetReq(struct R *pr, int cb, int cbAlign) { pr->cb = cb; pr->cbAlign = cbAlign; return pr; }
/** @sly2-annot addr=0x13BE58
 * Purpose: cid 0x67 vt14: LoadSoFromBrx into a stack-allocated entry table (0x580), then copy the used
 *   entries (0x2C each) to SW heap
 * Info: proposed name LoadBootyFromBrx (T1); class BOOTY slot14; TU booty
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest+parse-verified
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void func_0013BE58(struct S *p, void *a) {
    struct C ctx;
    struct R req;
    func_001AE478(&ctx);
    p->pv = func_001AE378(SetReq(&req, 0x580, 0x10));
    func_001F31E0(p, a);
    func_0013C440(p);
    p->pv = func_001AE238(SetReq(&req, p->c * 0x2c, 0x10), p->pv);
    func_001AE4A0(&ctx, 2);
}
