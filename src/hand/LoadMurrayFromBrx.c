struct REQ { int cb; int align; };
struct T { int x0; void *x4; void *x8; void *xC; };
struct P { char pad[0x15F0]; void *x15F0; void *x15F4; void *x15F8; void *x15FC; char pad2[0x1610 - 0x1600]; struct T *x1610; struct T *x1614; };
extern char D_002C0270[];
void LoadStepFromBrx(struct P *p);
void BindLoOidTable(struct P *p, int n, void *q);
struct T *func_001AE2A0(struct REQ *r);
void *PxfFindSo(struct P *p, int a, int b);
void func_00186930(struct T *t);
static __inline__ struct T *NewT(void) { struct REQ req; req.cb = 0xE0; req.align = 0x10; return func_001AE2A0(&req); }
/** @sly2-annot addr=0x1B4888
 * Purpose: MURRAY vt14: STEP LoadFromBrx 0x2039C0, BindLoOidTable(p,0x89,D_002C0270), allocates two 0xE0
 *   hand/grip records looking up child XFs 0x200-0x205 (PxfFindSo), caches grip XFs at x15F0..x15FC
 * Info: proposed name LoadMurrayFromBrx (T1); class MURRAY slot14; TU murray
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5b_rest+parse-verified
 * See: docs/engine/player.md#11-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void LoadMurrayFromBrx(struct P *p)
{
    LoadStepFromBrx(p);
    BindLoOidTable(p, 0x89, D_002C0270);
    p->x1610 = NewT();
    p->x1610->x4 = PxfFindSo(p, 0x200, 6);
    p->x1610->x8 = PxfFindSo(p, 0x201, 6);
    p->x1610->xC = PxfFindSo(p, 0x202, 1);
    func_00186930(p->x1610);
    p->x1614 = NewT();
    p->x1614->x4 = PxfFindSo(p, 0x203, 6);
    p->x1614->x8 = PxfFindSo(p, 0x204, 6);
    p->x1614->xC = PxfFindSo(p, 0x205, 1);
    func_00186930(p->x1614);
    p->x15F0 = p->x1610->xC;
    p->x15F4 = PxfFindSo(p, 0x202, 6);
    p->x15F8 = p->x1614->xC;
    p->x15FC = PxfFindSo(p, 0x205, 6);
}
