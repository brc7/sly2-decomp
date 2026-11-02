struct LM { float gMin; float gMax; };
struct SAA { char pad[0xC]; struct LM lm1; struct LM lm2; unsigned short c; };
struct P { char pad[0x1C]; int cframe; char pad1[0x50 - 0x20]; struct LM lm1; struct LM lm2; float g60; float g64; float g68; float g6C; };
void func_001ED8E0(struct P *p, struct SAA *psaa);
float GRandInRange__Fff(float gLow, float gHigh);
/** @sly2-annot addr=0x1EDB98
 * Purpose: LOOP vt7 Init
 * Info: proposed name InitLoop (T1); class LOOP slot7; TU shdanim
 * Notes: names_v2 C proposed PostLoopLoad__FP4LOOP (0.638)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void InitLoop(struct P *p, struct SAA *psaa)
{
    struct LM *plm2 = &p->lm2;
    struct LM *plm1;
    float g;
    func_001ED8E0(p, psaa);
    plm1 = &p->lm1;
    { float a = psaa->lm1.gMin; float b = psaa->lm1.gMax; p->lm1.gMin = a; plm1->gMax = b; }
    { float a = psaa->lm2.gMin; float b = psaa->lm2.gMax; p->lm2.gMin = a; plm2->gMax = b; }
    p->g64 = psaa->c;
    p->g60 = p->cframe / GRandInRange__Fff(p->lm1.gMin, plm1->gMax);
    g = GRandInRange__Fff(p->lm2.gMin, plm2->gMax);
    p->g6C = p->g68 = g;
}
