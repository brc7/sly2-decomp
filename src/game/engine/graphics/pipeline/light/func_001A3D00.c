// Give an object a slot from the 256-entry free bitmap (if it has none yet), then insert it into the
// tree or, when it can't be inserted now, mark its slot as pending.
typedef int qword __attribute__((mode(TI)));

struct NODE { qword plane; struct NODE *left, *right; int leftLeaf, rightLeaf; };
struct OBJ {
    char pad000[0xE0];
    int xE0;
    char pad0E4[0x118 - 0xE4];
    int slot;           // 0x118, < 0 until a slot is assigned
    char pad11C[0x15C - 0x11C];
    int x15C;
    int x160;
    int pad164;
    float radius;       // 0x168
};
struct SLOTS {
    char pad0000[0x4154];
    struct OBJ *objects[0x100];   // 0x4154
    int disabled;                 // 0x4554
    int pad4558;
    struct NODE *root;            // 0x455C
    int pad4560;
    void *leaves;
    int pad4568, pad456C;
    int pending[8];               // 0x4570, bit per slot
    unsigned int free[8];         // 0x4590, bit per free slot
};

qword func_001A62E8(void *p);
void func_001A3B80(struct SLOTS *s, struct NODE *node, qword pos, float radius, int slot);

static __inline__ void SetBit(int *a, int bit)
{
    int *pw = &a[bit / 32];
    int m = 1 << bit;
    *pw |= m;
}

/** @sly2-annot addr=0x1A3D00
 * Purpose: Allocate a slot from the 256-entry free bitmap if none, insert into the light tree or mark pending
 * Info: proposed name AddLightToSw (T1); TU light
 * Notes: names_v2 C proposed UpdateLightBeamGrfzon__FP5LIGHT (0.638)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3D00(struct SLOTS *s, struct OBJ *o)
{
    if (o->slot < 0) {
        unsigned int i;
        // w is initialised (live before the loop), so cse keeps w, not bit, as the register for
        // the low bit; the and is then folded into w and the free word is really re-read below.
        unsigned int w = 0, bit;
        int n, slot;
        for (i = 0; ; i++) {
            if (i >= 8)
                return;
            w = s->free[i];
            if (!w)
                continue;
            bit = -w & w;
            w = bit;
            s->free[i] &= ~w;
            n = i * 32;
            if (w & 0xAAAAAAAA) n += 1;
            if (w & 0xCCCCCCCC) n += 2;
            if (w & 0xF0F0F0F0) n += 4;
            if (w & 0xFF00FF00) n += 8;
            if (w & 0xFFFF0000U) slot = n + 16; else slot = n;
            break;
        }
        s->objects[slot] = o;
        o->slot = slot;
    }
    if (s->disabled)
        return;
    if (o->x15C)
        return;
    if (!o->xE0 || o->x160 || !s->root)
        SetBit(s->pending, o->slot);
    else
        func_001A3B80(s, s->root, func_001A62E8(o), o->radius, o->slot);
}
