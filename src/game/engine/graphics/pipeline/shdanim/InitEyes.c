struct SAAF { char pad[0xC]; float g0; float g1; float g2; float g3; int n1C; };
struct EYES { char pad[0x50]; float g0; float g1; float g2; float g3; int n60; int n64; };
void func_001ED8E0(struct EYES *p, struct SAAF *psaaf);
void SetEyesEyess__FP4EYES5EYESS(struct EYES *p, int eyess);
/** @sly2-annot addr=0x1EE090
 * Purpose: EYES vt7 Init
 * Info: proposed name InitEyes (T1); class EYES slot7; TU shdanim
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void InitEyes(struct EYES *p, struct SAAF *psaaf)
{
    func_001ED8E0(p, psaaf);
    p->n60=psaaf->n1C;
    p->g0=psaaf->g0;
    { float a=psaaf->g1; float b=psaaf->g2; p->g1=a; p->g2=b; }
    p->g3=psaaf->g3;
    p->n64=-1;
    SetEyesEyess__FP4EYES5EYESS(p,0);
}
