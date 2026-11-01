typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct N { qword plane; struct N *x10; struct N *x14; int x18; int x1C; };
struct S { char pad[0x4564]; char *x4564; };
void func_002564B8(void *p);
struct BS { int a[8]; };
static __inline__ void SetBit(int *a, int bit) { int *pw = &a[bit / 32]; *pw |= 1 << bit; }
/** @sly2-annot addr=0x1A3B80
 * Purpose: Recursive: descend the light BSP by plane distance vs radius and set the light's slot bit in leaf
 *   masks
 * Info: proposed name InsertLightInTree (T2); TU light
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3B80(struct S *s, struct N *n, qword pos, float r, int bit)
{
    U d, one;
    qword plane;
    float dist;
    plane = n->plane;
    func_002564B8(&d);
    func_002564B8(&one);
    __asm__("vmul.xyz %1, %3, %4\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddaz.x $ACC, %0, %1z\n\tvmsubw.x %1, %0, %3w\n\tqmfc2.ni %2, %1"
            : "=&j"(one.q), "=&j"(d.q), "=r"(dist) : "j"(plane), "j"(pos));
    if (dist < r) {
        if (n->x10)
            func_001A3B80(s, n->x10, pos, r, bit);
        else
            SetBit(((struct BS *)s->x4564)[n->x18].a, bit);
    }
    if (-r < dist) {
        if (n->x14)
            func_001A3B80(s, n->x14, pos, r, bit);
        else
            SetBit(((struct BS *)s->x4564)[n->x1C].a, bit);
    }
}
