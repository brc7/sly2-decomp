class CFrame {
    int m_a[4];
public:
    CFrame();
    ~CFrame();
};
struct LOC { int q0, q1, q2, q3; int f; int pad[3]; };
struct L2 { int a; int b; int pad[6]; };
struct B { int pad; int k4; char pad2[0xC4 - 8]; int kC4; };
extern "C" {
void OriginateParticles__FiP5EMITBP7EMITGEN(void *a, B *b, LOC *ploc);
void EmitRips__FP5EMITBP5EMITGiP6VECTORT3PfT5(B *b, void *c, void *a, int q0, int q1, int q2, int q3);
void OriginateSplineSinkParticles(void *a, B *b, LOC *ploc, L2 *pl2);
void EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(B *b, void *c, void *a, int q0, int q1, int q2, int q3, int x, int y);
void EmitParticles__FiP5EMITBP5EMITG(void *a, B *b, void *c)
{
    if (a) {
        LOC loc;
        CFrame frame;
        loc.f = 1;
        OriginateParticles__FiP5EMITBP7EMITGEN(a, b, &loc);
        if (b->k4 == 1) {
            EmitRips__FP5EMITBP5EMITGiP6VECTORT3PfT5(b, c, a, loc.q0, loc.q1, loc.q2, loc.q3);
        } else {
            L2 l2;
            if (b->kC4 == 2) {
                OriginateSplineSinkParticles(a, b, &loc, &l2);
            } else {
                l2.a = 0;
                l2.b = 0;
            }
            EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(b, c, a, loc.q0, loc.q1, loc.q2, loc.q3, l2.a, l2.b);
        }
    }
}
}
