// Change a selector's input mode and, when the joystick points somewhere, move the selection to the
// enabled item that lies closest along that direction from the current one.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };

struct ITEM {
    int id;
    char pad04[0x10];
    float x, y;     // 0x14, 0x18
    int flags;      // 0x1C, bit 2 = selectable
};
struct INPUT { char pad[0x103C]; float x, y; };
struct SELECTOR {
    char pad00[0x10];
    INPUT *input;   // 0x10
    char pad14[0x14];
    int mode;       // 0x28
    float tChanged; // 0x2C
    char pad30[0x18];
    int selected;   // 0x48
    int count;      // 0x4C
    ITEM *items;    // 0x50
};
struct PROX { float along, distance; };
struct HIT { qword position, other, normal; char pad30[0x10]; float fraction; char pad44[0x1C]; };

// D_002DDF0C is read as a field of the clock struct: a struct field at a fixed address may alias
// sel->mode (a scalar global may not), which keeps the clock load after the mode store.
struct CLOCK { char pad[0x3C]; float t3C; };
extern CLOCK D_002DDED0;
extern float D_002C3280, D_002C3274, D_002C3278, D_002C327C, D_002A79C8;

extern "C" {
ITEM *func_001DAE20(SELECTOR *sel, int id);
int func_0014C748(qword point, qword start, qword end, int flags, HIT *hit, float radius);
void FindClosestPointOnLineSegment(qword point, qword start, qword end, qword *unused, float *along, float *distance);
void SelectGadgetMenuItem(SELECTOR *sel, int id);
void func_001DA9E8(SELECTOR *sel);
void *PsndStartSound(int id);
void SetSndSourceLo(void *sound, int flags);
void SetGadgetMenuStickDir(SELECTOR *sel, int mode);
}

static inline qword XYZ(float x, float y, float z, U &home)
{
    qword r, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(r), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    home.q = r;
    return home.q;
}
static inline qword ToVF(qword v) { qword r; __asm__("qmtc2.ni %1,%0" : "=j"(r) : "r"(v)); return r; }
static inline qword Zero() { qword r; __asm__("vsub.xyzw %0,$vf0,$vf0" : "=j"(r)); return r; }
static inline qword Normalize(qword a)
{
    qword d, one, r, l;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=j"(d), "=&j"(one) : "j"(a));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(l) : "j"(d));
    __asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q" : "=j"(r) : "j"(a), "j"(l));
    return r;
}
// "rV": the float stays a memory operand until reload, which loads it through the dying %hi register.
static inline qword Splat(const float &f, U &home)
{
    qword s;
    __asm__("qmtc2.ni %1,%0" : "=j"(s) : "rV"(*(const int *)&f));
    home.q = s;
    return home.q;
}
static inline qword Scale(qword a, qword s) { qword r; __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(r) : "j"(a), "j"(s)); return r; }
// b by const reference: the ray lives in a stack slot, so the two Adds are not CSEd and each
// reloads it into the register the result goes to.
static inline qword Add(qword a, const qword &b) { qword r; __asm__("vadd.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }

/** @sly2-annot addr=0x1DB0C8
 * Purpose: Gadget menu slot 14: move the selection to the selectable item lying closest along the stick
 *   direction
 * Info: proposed name SetGadgetMenuStickDir (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SetGadgetMenuStickDir(SELECTOR *sel, int mode)
{
    U home;
    HIT hit;
    PROX proximity;
    if (mode == sel->mode)
        return;
    sel->mode = mode;
    sel->tChanged = D_002DDED0.t3C;
    if (mode == -1 || sel->selected == -1)
        return;
    ITEM *current = func_001DAE20(sel, sel->selected);
    qword position = XYZ(current->x, current->y * D_002C3280, 0.0f, home);
    qword direction;
    if (sel->input)
        direction = ToVF(XYZ(sel->input->x, -sel->input->y, 0.0f, home));
    else
        direction = Zero();
    qword ray = Scale(Normalize(direction), Splat(D_002C3274, home));
    int bestId = -1;
    float best = D_002A79C8;
    for (int i = 0; i < sel->count; i++) {
        qword origin = ToVF(position);
        ITEM *item = &sel->items[i];
        if (item == current)
            continue;
        if (!(item->flags & 4))
            continue;
        float zero = 0.0f;
        qword point = XYZ(item->x, item->y * D_002C3280, zero, home);
        float score;
        if (func_0014C748(point, origin, Add(origin, ray), 1, &hit, D_002C3278) > 0) {
            score = hit.fraction * D_002C3274;
        } else {
            FindClosestPointOnLineSegment(point, origin, Add(origin, ray), 0, &proximity.along, &proximity.distance);
            if (proximity.distance < D_002C327C) {
                if (zero < proximity.along)
                    score = D_002C3274 + proximity.distance;
                else
                    continue;
            } else
                continue;
        }
        if (score < best) {
            bestId = item->id;
            best = score;
        }
    }
    if (bestId != -1) {
        SelectGadgetMenuItem(sel, bestId);
        func_001DA9E8(sel);
        SetSndSourceLo(PsndStartSound(0x35), 0);
    }
}
