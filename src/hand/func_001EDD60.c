struct LM { float gMin; float gMax; };
struct SAA { char pad[0xC]; struct LM lm1; struct LM lm2; unsigned short c; };
struct P { char pad[0x1C]; int cframe; char pad1[0x50 - 0x20]; struct LM lm1; struct LM lm2; float g60; float g64; float g68; float g6C; };
void func_001ED8E0(struct P *p, struct SAA *psaa);
float GRandInRange__Fff(float gLow, float gHigh);
/** @sly2-annot addr=0x1EDD60
 * Purpose: PINGPONG vt7 Init
 * Info: proposed name InitPingpong (T1); class PINGPONG slot7; TU shdanim
 * Notes: names_v2 B proposed PostPingpongLoad__FP8PINGPONG (0.856)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void func_001EDD60(struct P *p, struct SAA *psaa)
{
    struct LM *plm2 = &p->lm2;
    struct LM *plm1;
    float g;
    func_001ED8E0(p, psaa);
    plm1 = &p->lm1;
    { float a = psaa->lm1.gMin; float b = psaa->lm1.gMax; p->lm1.gMin = a; plm1->gMax = b; }
    { float a = psaa->lm2.gMin; float b = psaa->lm2.gMax; p->lm2.gMin = a; plm2->gMax = b; }
    p->g64 = psaa->c;
    p->g60 = (p->cframe * 2) / GRandInRange__Fff(p->lm1.gMin, plm1->gMax);
    g = GRandInRange__Fff(p->lm2.gMin, plm2->gMax);
    p->g6C = p->g68 = g;
}
