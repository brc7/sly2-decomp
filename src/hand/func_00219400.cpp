// func_00219400: apply one animation key's poses to the reuse slot's items (sprite animation, see func_00219FE0).
// For every pose of key `phase`: reset the item's "previous" state (colours default to additive 0x7F7F7F7F / multiply
// 0xFFFFFFFF unless the pose has a COLORS entry), look in the following keys for the same item to find its "next"
// pose, then interpolate matrix and colours between them at the clamped time (GLimitLm), combine with the parent
// matrix/colours (func_00218790) and draw through func_00215850 (same sprite) or replace the sprite (func_002158C0 +
// func_00215730). Finally the item's matrix/colours/pose bytes are saved and the link kinds are set (1 for the
// linking pose, 4 for the item it links to).
//
// STAND-IN (see LoadOptionFromBrx__FPvP5EOPIDiP18CBinaryInputStream.cpp for the same trick): the spilled gcse reaching pseudos (j+1, &item->flags, &outputAdditive
// and the hoisted frame addresses) get their spill slots/registers from the gcse expression hash-table size
// S = max_cuid/2|1. This spelling has S = 279 after cse1; the game's order needs S = 277/283/289. `if (fNever)` is dead code
// (cprop folds it, jump deletes it; the final code is unchanged) that only supplies the missing insns (S = 283).
// What the game really had there is unknown.
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
struct RGBA_RAW { uchar r, g, b, a; };
struct RGBA {
    uchar r, g, b, a;
    RGBA() {}
    RGBA(const RGBA &c) { *(RGBA_RAW *)this = *(const RGBA_RAW *)&c; }
    RGBA(const RGBA_RAW &c) { *(RGBA_RAW *)this = c; }
    RGBA &operator=(const RGBA &c) { *(RGBA_RAW *)this = *(const RGBA_RAW *)&c; return *this; }
} __attribute__((aligned(16)));
struct M23 { float a[6]; } __attribute__((aligned(16)));
struct SPRITE { int id, kind; void *group; int fieldC, index; void *extra; char pad18[0x28]; };
struct ITEM {
    RGBA_RAW additive, multiply;
    char pad8[8];
    M23 matrix;
    SPRITE *sprite;
    int kind;
    float previousTime;
    RGBA_RAW previousAdditive, previousMultiply;
    M23 *previousMatrix;
    float nextTime;
    RGBA_RAW nextAdditive, nextMultiply;
    M23 *nextMatrix;
    int index;
    float cachedTime;
    uchar interpolate, mode;
    char pad62[2];
    int field64;
    int flags, pad6C;
};
struct REUSE { int count; ITEM *items; ushort index, phase; float time; int field10, field14; };
struct ANIM;
struct GROUP {
    ushort animCount, reuseCount;
    ANIM *animations;
    REUSE *reuse;
    char padC[0x14];
    int index;
    void *owner;
    int first, pad2C;
};
struct KEYPOSE {
    ushort id, index, offset;
    short color;
    char pad8[8];
    M23 matrix;
    uchar interpolate, mode;
    char pad32[2];
    int selected;
    char pad38[8];
};
struct COLORS { RGBA_RAW multiply, additive; };
struct TAG { int kind, value; };
struct KEY {
    float time;
    uchar itemCount, colorCount, shortCount, byteCount;
    uchar *bytes;
    KEYPOSE *items;
    COLORS *colors;
    ushort *shorts;
    uchar tagCount;
    char pad19[3];
    TAG *tags;
};
struct ROOT { char pad[0x54]; SPRITE *sprites; };
struct ANIM { ushort count, pad2; KEY *keys; float duration; ROOT *owner; GROUP *group; int id; };
extern "C" {
void ClearSle(int *);
void *func_002333B4(void *, int, unsigned int);
void func_00215730(SPRITE *, ITEM *, M23 *, RGBA, RGBA);
void func_00219400(float, float, ANIM *, int, int, M23 *, const RGBA *, const RGBA *);
void func_002158C0(SPRITE *, ITEM *);
void func_00218790(RGBA *, RGBA *, RGBA *, RGBA *, RGBA *, RGBA *);
void UpdateMatchingSprite(float, SPRITE *, ITEM *, M23 *, RGBA, RGBA) __asm__("func_00215850");
void UpdateCurrentSprite(float, SPRITE *, ITEM *, M23 *, RGBA, RGBA) __asm__("func_00215850");
}

// Lerp one colour channel; the +0.999 rounds towards the far end.
static inline int NLerp(float t, int previous, int next)
{
    if (previous == next)
        return next;
    if (previous < next)
        return previous + (int)(t * ((float)next + 0.999f - (float)previous));
    return next + (int)((1.0f - t) * ((float)previous + 0.999f - (float)next));
}

static inline RGBA BlendColors(float t, RGBA previous, RGBA next)
{
    RGBA result;
    result.r = NLerp(t, previous.r, next.r);
    result.g = NLerp(t, previous.g, next.g);
    result.b = NLerp(t, previous.b, next.b);
    result.a = NLerp(t, previous.a, next.a);
    return result;
}

static inline M23 BlendMatrix(float t, const M23 &previous, const M23 &next)
{
    M23 result;
    for (int n = 0; n < 6; n++)
        result.a[n] = (1.0f - t) * previous.a[n] + t * next.a[n];
    return result;
}

static inline M23 Mul(const M23 &q, const M23 &n)
{
    float m0 = q.a[0], m1 = q.a[1], m2 = q.a[2], m3 = q.a[3], m4 = q.a[4], m5 = q.a[5];
    M23 r;
    r.a[0] = m0 * n.a[0] + m1 * n.a[2];
    r.a[1] = m0 * n.a[1] + m1 * n.a[3];
    r.a[2] = m2 * n.a[0] + m3 * n.a[2];
    r.a[3] = m2 * n.a[1] + m3 * n.a[3];
    r.a[4] = m4 * n.a[0] + m5 * n.a[2] + n.a[4];
    r.a[5] = m4 * n.a[1] + m5 * n.a[3] + n.a[5];
    return r;
}

enum Uninitialized { Uninit };
struct LM {
    float lo, hi;
    LM(Uninitialized) {}
    LM() {}
};
float GLimitLm(LM *, float);

/** @sly2-annot addr=0x219400
 * Purpose: apply one animation key poses to items
 * Info: proposed name ApplySpriteKeyPoses (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void func_00219400(float time, float duration, ANIM *animation, int phase, int index, M23 *parent, const RGBA *additive, const RGBA *multiply)
{
    int fNever = 0;
    int j = 0;
    ITEM *items = animation->group->reuse[index].items;
    KEY *key = &animation->keys[phase];
    M23 matrix;
    RGBA outputAdditive, outputMultiply, currentAdditive, currentMultiply;

    for (; j < key->itemCount; j++) {
        KEYPOSE *pose = &key->items[j];
        int entryIndex = pose->index, linkOffset = pose->offset, id = pose->id;
        ITEM *item = (ITEM *)(entryIndex * sizeof(ITEM) + (int)items);
        // declared here (not folded into the stores): loop.c only hoists the two constants if they live across the colour test
        uint defaultAdditive = 0x7F7F7F7F, defaultMultiply = 0xFFFFFFFF;
        COLORS *colors = 0;
        if (pose->color >= 0)
            colors = &key->colors[pose->color];
        uint additiveWord = defaultAdditive;
        *(uint *)&item->previousAdditive = additiveWord;
        *(uint *)&currentAdditive = additiveWord;
        uint multiplyWord = defaultMultiply;
        *(uint *)&item->previousMultiply = multiplyWord;
        *(uint *)&currentMultiply = multiplyWord;
        if (colors) {
            item->previousAdditive = colors->additive;
            *(RGBA_RAW *)&currentAdditive = item->previousAdditive;
            item->previousMultiply = colors->multiply;
            *(RGBA_RAW *)&currentMultiply = item->previousMultiply;
        }
        item->previousMatrix = &pose->matrix;
        item->previousTime = key->time;
        item->nextMatrix = 0;
        item->nextTime = 0.0f;

        // find this item's pose in a later key
        if (pose->interpolate) {
            int nextKey = phase + 1;
            if (nextKey < animation->count) {
                do {
                    KEY *future = &animation->keys[nextKey];
                    int afterKey = nextKey + 1;
                    for (int n = 0; n < future->itemCount; n++) {
                        KEYPOSE *futurePose = (KEYPOSE *)(n * sizeof(KEYPOSE) + (int)future->items);
                        if (futurePose->index == entryIndex) {
                            item->nextMatrix = &futurePose->matrix;
                            item->nextTime = future->time;
                            int futureColor = future->items[n].color;
                            if (futureColor >= 0) {
                                item->nextAdditive = future->colors[futureColor].additive;
                                item->nextMultiply = future->colors[futureColor].multiply;
                            } else {
                                item->nextAdditive = item->previousAdditive;
                                item->nextMultiply = item->previousMultiply;
                            }
                            break;
                        }
                    }
                    nextKey = afterKey;
                } while (nextKey < animation->count && !item->nextMatrix);
            }
        }

        if (pose->interpolate && item->nextMatrix && item->previousTime < item->nextTime) {
            LM copy(Uninit);
            float limited;
            {
                LM bounds;
                LM *pb = &bounds;
                func_002333B4(pb, 0, 8);
                bounds.lo = item->previousTime;
                bounds.hi = item->nextTime;
                copy = bounds;
                limited = GLimitLm(&copy, duration);
            }
            float fraction = (limited - item->previousTime) / (item->nextTime - item->previousTime);
            matrix = BlendMatrix(fraction, *item->previousMatrix, *item->nextMatrix);
            currentAdditive = BlendColors(fraction, RGBA(item->previousAdditive), RGBA(item->nextAdditive));
            currentMultiply = BlendColors(fraction, RGBA(item->previousMultiply), RGBA(item->nextMultiply));
        } else {
            matrix = *item->previousMatrix;
            *(RGBA_RAW *)&currentAdditive = item->previousAdditive;
            *(RGBA_RAW *)&currentMultiply = item->previousMultiply;
        }
        matrix = Mul(matrix, *parent);
        {
            RGBA inputMultiply(*multiply), inputAdditive(*additive), localMultiply(currentMultiply), localAdditive(currentAdditive);
            func_00218790(&inputMultiply, &inputAdditive, &localMultiply, &localAdditive, &outputMultiply, &outputAdditive);
        }
        if (id > 0) {
            if (item->sprite) {
                if (item->sprite->id == id) {
                    UpdateMatchingSprite(duration, item->sprite, item, &matrix, outputAdditive, outputMultiply);
                } else {
                    func_002158C0(item->sprite, item);
                    func_00215730(&animation->owner->sprites[id], item, &matrix, outputAdditive, outputMultiply);
                }
            } else {
                func_00215730(&animation->owner->sprites[id], item, &matrix, outputAdditive, outputMultiply);
            }
        } else {
            UpdateCurrentSprite(duration, item->sprite, item, &matrix, outputAdditive, outputMultiply);
        }
        item->cachedTime = time;
        item->matrix = matrix;
        item->additive = *(RGBA_RAW *)&outputAdditive;
        item->multiply = *(RGBA_RAW *)&outputMultiply;
        item->interpolate = pose->interpolate;
        item->mode = pose->mode;
        item->field64 = pose->selected;
        ClearSle(&item->flags);
        if (linkOffset) {
            item->kind = 1;
            items[entryIndex + linkOffset].kind = 4;
        } else if (item->kind != 4)
            item->kind = 0;
        if (fNever)
            ClearSle(0);
    }
}
