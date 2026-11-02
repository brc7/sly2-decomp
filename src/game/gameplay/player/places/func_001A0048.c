typedef int qword __attribute__((mode(TI)));
typedef struct { union { qword q0; struct { void *x0; int x4; void *x8; int xc; } s; } u; qword q1; } E;
struct S { int x0; int c; int pad[2]; E a[1]; };
/** @sly2-annot addr=0x1A0048
 * Purpose: Compact the place table removing entries that reference the given object(s)
 * Info: proposed name RemoveJtPlacesOf (T2); TU places
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#places-0x19f3e8-0x1a0300
 * @end-sly2-annot */
void func_001A0048(struct S *s, void *p0, void *p8)
{
    E *dst = s->a;
    E *src = dst;
    E *end = &s->a[s->c];
    for (; src < end; src++) {
        if (p0 && p0 == src->u.s.x0)
            continue;
        if (p8 && p8 == src->u.s.x8)
            continue;
        if (src != dst)
            *dst = *src;
        dst++;
    }
    s->c = dst - s->a;
}
