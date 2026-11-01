struct Q { unsigned int x0; char pad[0x30]; int x34; };
struct S { int x0; int x4; };
/** @sly2-annot addr=0x1A2160
 * Purpose: Hurt states 5,8..12 clear x34; always set bit 0 of grf
 * Info: proposed name AddJtHurtCpdefiFlags (T2); TU hurt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_001A2160(struct S *p, struct Q *q)
{
    switch (p->x4) {
    case 5: case 8: case 9: case 10: case 11: case 12: q->x34 = 0; break; case 4: break;
    }
    q->x0 |= 1;
}
