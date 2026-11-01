struct Q { char pad[0x118]; int x118; };
struct S { char pad[0x4154]; int a4154[0x100]; int x4554; int pad4558[14]; unsigned int bits[0x10]; };
/** @sly2-annot addr=0x1A3E88
 * Purpose: Free the light's slot bit, clear table entry, slot=-1, mark tree dirty (+0x4554)
 * Info: proposed name RemoveLightFromSw (T1); TU light
 * Notes: names_v2 C proposed NCmpFindLight__FPP5LIGHTT0 (0.638)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3E88(struct S *p, struct Q *q)
{
    int n = q->x118;
    p->bits[n / 32] |= 1 << (n % 32);
    p->a4154[q->x118] = 0;
    q->x118 = -1;
    p->x4554 = 1;
}
