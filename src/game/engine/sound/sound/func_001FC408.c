struct R { float lo, hi; };
struct E { int n; struct R r; float g; };
struct P { char pad[0xA0]; int c[6]; struct E ae[6][8]; };
void func_001FB658(struct P *, int, int);
/** @sly2-annot addr=0x1FC408
 * Purpose: Append (object, volume, range) to sub-group k (max 8 per group); positional flag 0x200
 * Info: proposed name AddStreamsndGroupSource (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#amb-0x12eb48-0x12f0f0,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC408(struct P *p, unsigned int k, int n, struct R *pr, float g)
{
    if (k < 6 && p->c[k] < 8) {
        struct E *pe;
        func_001FB658(p, 0x200, 0x200);
        pe = &p->ae[k][p->c[k]++];
        pe->n = n;
        pe->g = g;
        pe->r = *pr;
        if (pe->r.lo == pe->r.hi) pe->r.hi += 1.0f;
    }
}
