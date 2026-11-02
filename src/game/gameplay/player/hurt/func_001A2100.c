extern float D_002BFB20, D_002BFB24;
struct Q { char pad[0x94]; float x94; float x98; };
struct S { int x0; int x4; };
/** @sly2-annot addr=0x1A2100
 * Purpose: Hurt states 4/12 set adjust fields x94=D_002BFB20, x98=0 (other states other values)
 * Info: proposed name AdjustJtHurtXp (T2); TU hurt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8,
 *   docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_001A2100(struct S *p, struct Q *q)
{
    switch (p->x4) {
    case 4: case 12:
        q->x94 = D_002BFB20;
        q->x98 = 0;
        break;
    case 0: case 6: case 7:
        break;
    default:
        q->x94 = D_002BFB20;
        q->x98 = D_002BFB24;
        break;
    }
}
