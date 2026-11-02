struct B { int a[3]; int c; };
struct S { int a[2]; struct B *pb; char pad[0xE4]; int f0; int f4; };
void PostRipEmit(struct S *a, struct S *b);
/** @sly2-annot addr=0x1CF5D0
 * Purpose: FLYING (Sly2 RIPT 3) PostEmit: base PostRipEmit + setup
 * Info: proposed name PostFlyingEmit (T1); TU rip
 * Notes: names_v2 v1 proposed InitFlake__FP5FLAKEP6VECTORfP2SO (0.001)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void PostFlyingEmit(struct S *a, struct S *b)
{
    PostRipEmit(a, b);
    a->f0 = b->pb->c;
    a->f4 = b->f0;
}
