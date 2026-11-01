/* func_001CE2E8 (Claude agent, wave-3 lane 6): for an active particle system (mode 1), walk the subject nodes and their shape lists; for
 * every usable shape query its tree for candidate entries (scratch-frame allocation), then let each live rip test its contacts against the
 * shape (vtable slot 8, stop at the first hit); finally notify the owner (vtable slot 9) if the rip list reports a change.
 *
 * What made it match:
 *  - the hit loop is `if (i < count) do { XP *hit = &contacts[i]; ... i++; } while (i < count);`: loop.c turns `hit` into one pointer giv
 *    (fresh `addiu a2,sp,0x30` init, all three fields as offsets) so &contacts is not hoisted out of the shape loop, and the `i < count`
 *    guard gives i the extra reference that puts it in $s2 ahead of count;
 *  - the rip loop is a real for loop (loop depth weights the rip/hit allocation priorities);
 *  - the tree origin is read through a const-reference accessor: that load is not a struct-member MEM any more, so it may alias the
 *    `entries` store, which sched1 then issues right after the allocation call (the target's store order and $v0 for p->state);
 *  - func_00118A88 uses the matched callee's argument order (pos, eps, count, entries).
 */
typedef int qword __attribute__((mode(TI)));
struct DL { void *head, *tail; int offset; };
struct DLI { DL *list; void **next; int offset; DLI *outer; };
struct ENTRY { int a, b; void *poly; };
struct SHAPE { char pad10[0x10]; unsigned long flags; char pad18[0x54]; int capacity; char pad70[4]; void *tree; };
struct XP { qword position, other, normal; char pad30[0x10]; float fraction; char pad44[0x1C]; };
struct RIP;
struct RVE { short delta, pad; int (*fn)(void *, SHAPE *, qword, qword); };
struct RIP { char pad14[0x14]; float start; char pad18[0x58]; qword position, extra; char pad90[0x5C]; RVE *vt; };
struct NODE;
struct STATE { char pad20[0x20]; qword origin; float radius; DL shapes; char pad40[0x20]; void *subject; NODE *nodes; char pad68[8]; unsigned long flags; };
struct OBJ { char padEC[0xEC]; STATE *state; };
struct NODE { int pad; NODE *next; char pad8[4]; OBJ *object; };
struct PVE { short delta, pad; void (*fn)(void *); };
struct P { char padC[0xC]; PVE *vt; char pad10[0xDC]; STATE *state; char padF0[0x60]; int mode; char pad154[8]; DL rips; };
struct CLOCK { float t, dt; };
struct REQ { int cb, align; };
struct FRAME { int a[4]; };
extern "C" {
extern DLI *D_002DD2FC; /* innermost active list iterator */
extern CLOCK D_002DDED0;
FRAME *func_001AE478(FRAME *);
void func_001AE4A0(FRAME *, int);
void *func_001AE318(const REQ *);
int func_00118A88(void *, int, qword, float, int *, ENTRY **);
int func_0014C6D8(SHAPE *, ENTRY *, qword, qword, int, XP *);
int func_00132B10(void *, int);
int func_0011A3B0(DL *);
void func_001CE2E8(P *);
}
/* the scratch request and the rip iterator share one stack slot */
union PHASE { REQ request; DLI rip; };
static inline void Init(DLI *it, DL *list) { it->list = list; it->next = (void **)list; it->offset = list->offset; it->outer = D_002DD2FC; D_002DD2FC = it; }
static inline void *Next(DLI *it) { void *p = *it->next; it->next = (void **)((char *)p + it->offset); return p; }
static inline void SetAlign(REQ *r, int a) { r->align = a; }
static inline const qword &Origin(STATE *s) { return s->origin; }
static inline int Min(int a, int b) { return a <= b ? a : b; }
/** @sly2-annot addr=0x1CE2E8
 * Purpose: Collide live rips with nearby SO shapes (scratch stack alloc, BSP query) calling each rip's
 *   FBounce virtual
 * Info: proposed name BounceRipgRips (T1); TU rip
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001CE2E8(P *p)
{
    if (p->mode == 1) {
        NODE *node;
        for (node = p->state->nodes; node; node = node->next) {
            if (node->object->state->flags & 4) continue;
            int special = func_00132B10(node->object->state->subject, 0x59);
            DLI shapes;
            Init(&shapes, &node->object->state->shapes);
            SHAPE *shape;
            while ((shape = (SHAPE *)Next(&shapes)) != 0) {
                if ((shape->flags & (1UL << 48)) && !special) continue;
                {
                    FRAME frame;
                    func_001AE478(&frame);
                    PHASE phase;
                    XP contacts[4];
                    int remaining;
                    ENTRY *entries;
                    entries = 0;
                    if (!(shape->flags & (1UL << 49))) {
                        remaining = Min(0x1555, shape->capacity);
                        phase.request.cb = remaining * 12;
                        SetAlign(&phase.request, 16);
                        entries = (ENTRY *)func_001AE318(&phase.request);
                        if (func_00118A88(shape->tree, 0x7FFFFFFF, Origin(p->state), p->state->radius, &remaining, &entries) != 2) {
                            func_001AE4A0(&frame, 2);
                            continue;
                        }
                        entries--;
                    }
                    Init(&phase.rip, &p->rips);
                    RIP *rip;
                    for (rip = (RIP *)Next(&phase.rip); rip; rip = (RIP *)Next(&phase.rip)) {
                        if (D_002DDED0.t <= rip->start) continue;
                        int count = func_0014C6D8(shape, entries, rip->extra, rip->position, 4, contacts);
                        int i = 0;
                        if (i < count) do {
                            XP *hit = &contacts[i];
                            if (hit->fraction != 0.0f && rip->vt[8].fn((char *)rip + rip->vt[8].delta, shape, hit->position, hit->normal)) break;
                            i++;
                        } while (i < count);
                    }
                    D_002DD2FC = phase.rip.outer;
                    func_001AE4A0(&frame, 2);
                }
            }
            D_002DD2FC = shapes.outer;
        }
        if (func_0011A3B0(&p->rips)) p->vt[9].fn((char *)p + p->vt[9].delta);
    }
}
