struct S8 { float a, b; };
struct X { char pad[0x1120]; struct S8 a, b, c; };
/** @sly2-annot addr=0x143090
 * Purpose: Copy three 8-byte pairs into the struct
 * Info: proposed name SetStepplayerIdleParams (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00143090(struct X *p, struct S8 *a, struct S8 *b, struct S8 *c)
{
    p->a = *a;
    p->b = *b;
    p->c = *c;
}
