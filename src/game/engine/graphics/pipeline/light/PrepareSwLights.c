struct S { char pad[0x4154]; void *a[256]; int x4554; int x4558, x455c; int x4560; void *x4564; int x4568, x456c; char x4570[0x20]; };
void func_002333B4(void *p, int c, int n);
void AddLightToSw(struct S *s, void *p);
/** @sly2-annot addr=0x1A3EE8
 * Purpose: If the light tree is dirty, clear masks and re-insert every slotted light
 * Info: proposed name PrepareSwLights (T1); TU light
 * Notes: names_v2 v1 proposed SetLightKind__FP5LIGHT6LIGHTK (0.200)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void PrepareSwLights(struct S *s)
{
    unsigned int i;
    if (s->x4554) {
        s->x4554 = 0;
        func_002333B4(s->x4570, 0, 0x20);
        func_002333B4(s->x4564, 0, s->x4560 << 5);
        for (i = 0; i < 0x100; i++) {
            if (s->a[i])
                AddLightToSw(s, s->a[i]);
        }
    }
}
