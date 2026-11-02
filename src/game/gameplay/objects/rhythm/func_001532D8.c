struct E { int a; int b; int c; float g; float t; int z14; int z18; int z1C; int z20; };
struct S { char pad[0x1C0]; int c; struct E a[3]; };
extern char D_003DD040[];
float func_001FE3F8(void *p);
void *func_002332B8(void *dst, void *src, int cb);
/** @sly2-annot addr=0x1532D8
 * Purpose: Push cue {a,b,c,g, t=music time} into a ring of 3 at +0x1C0
 * Info: proposed name PushRhythmMgrCue (T2); TU rhythm
 * Confidence: low (draft) [T2 0.35] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_001532D8(struct S *p, int a, int b, int c, float g)
{
    struct E *pe;
    if (p->c == 3) {
        func_002332B8(&p->a[0], &p->a[1], (p->c - 1) * sizeof(struct E));
        pe = &p->a[p->c - 1];
    } else {
        pe = &p->a[p->c++];
    }
    pe->a = a;
    pe->b = b;
    pe->c = c;
    pe->g = g;
    pe->t = func_001FE3F8(D_003DD040);
    pe->z14 = 0;
    pe->z18 = 0;
    pe->z1C = 0;
    pe->z20 = 0;
}
