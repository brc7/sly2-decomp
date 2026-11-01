typedef int qword __attribute__((mode(TI)));
typedef struct { qword a[3]; } MATRIX3;
struct S { char pad[0x1D8]; float g; int pad2; MATRIX3 mat; };
extern qword D_002A2DC0;
MATRIX3 func_0011B500(qword v, float g);
/** @sly2-annot addr=0x1B1818
 * Purpose: Store heading and rotation matrix about up
 * Info: proposed name SetCid96Heading (T2); class CID96; TU tu_1B0B78
 * Notes: names_v2 C proposed SetStepGravityRatio__FP4STEPf (0.200)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
void func_001B1818(struct S *p, float g)
{
    p->g = g;
    p->mat = func_0011B500(D_002A2DC0, g);
}
