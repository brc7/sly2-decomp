struct A { char pad[0x28]; unsigned long x28; };
struct B { char pad[0x10]; unsigned long x10; };
extern char *D_002E1E40, *D_002DD5BC, *D_002F7900;
void func_001A0048(void *p, struct A *a, struct B *b);
/** @sly2-annot addr=0x1A00D0
 * Purpose: Purge a removed object from the place tables of Sly (g_pjt D_002E1E40), D_002DD5BC and D_002F7900
 *   (the other two player globals, presumably Bentley/Murray)
 * Info: proposed name RemoveSwPlacesOf (T1); TU places
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/player.md#places-0x19f3e8-0x1a0300
 * @end-sly2-annot */
void func_001A00D0(void *g, struct A *a, struct B *b)
{
    if (D_002E1E40)
        func_001A0048(D_002E1E40 + 0x9E0, a, b);
    if (D_002DD5BC)
        func_001A0048(D_002DD5BC + 0x9E0, a, b);
    if (D_002F7900)
        func_001A0048(D_002F7900 + 0x9E0, a, b);
    if (a)
        a->x28 &= ~0x0800000000000000UL;
    if (b)
        b->x10 &= ~0x0008000000000000UL;
}
