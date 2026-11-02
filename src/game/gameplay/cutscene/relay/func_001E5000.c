typedef int qword __attribute__((mode(TI)));
struct X { char pad[0x1044]; int n1044; float f1048; char pad2[0x1060 - 0x104C]; qword v1060; qword v1070; };
struct P { char pad[0x24]; struct X *p24; char pad2[0x74 - 0x28]; void *p74; };
extern qword D_002A2DD0;
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
qword func_001A62E8(struct X *);
void GetJsgTargetPosHeading(struct P *, void *, qword *, int);
/** @sly2-annot addr=0x1E5000
 * Purpose: Set the script focus object (+0x74) and aim the STEPPLAYER actor's look target at it (STEPPLAYER
 *   +0x1044/+0x1060/+0x1070)
 * Info: proposed name SetJsgFocus (T1); TU relay
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E5000(struct P *p, void *pv)
{
    struct X *px;
    if (p->p24 && FIsBasicDerivedFrom__FP5BASIC3CID(p->p24, 0x34))
        px = p->p24;
    else
        px = 0;
    if (p->p74) {
        if (px) {
            qword v = px->v1060, q, d;
            px->n1044 = 0;
            q = func_001A62E8(px);
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(v), "j"(q));
            px->v1070 = d;
            ((float *)&px->v1070)[2] = 0.0f;
        }
        p->p74 = 0;
    }
    if (!pv) return;
    if (px) {
        px->n1044 = 1;
        GetJsgTargetPosHeading(p, pv, &px->v1060, 0);
        ((float *)&px->v1060)[2] += px->f1048;
        px->v1070 = D_002A2DD0;
    }
    p->p74 = pv;
}
