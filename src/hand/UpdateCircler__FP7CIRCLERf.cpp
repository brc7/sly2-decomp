struct CLOCK { float t; };
extern CLOCK D_002DDED0 asm("D_002DDED0");
struct SAI { int a; };
struct CIRCLER { char pad[0x34]; SAI sai; char pad2[0x50 - 0x38]; float radsSpeed; float radius; float duCenter; float dvCenter; };
extern "C" int func_0011A1E8(SAI *psai);
extern "C" float func_0011CF60(float rad);
extern "C" void func_0011D608(float rad, float *pgSin, float *pgCos);
extern "C" void func_001EDAD8(CIRCLER *p, float du, float dv);
/** @sly2-annot addr=0x1EE6D8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void UpdateCircler(CIRCLER *pcircler, float dt)
{
    if (func_0011A1E8(&pcircler->sai))
        return;

    float angle = func_0011CF60(D_002DDED0.t * pcircler->radsSpeed);

    float sinOut;
    float cosOut;
    func_0011D608(angle, &sinOut, &cosOut);

    sinOut = (sinOut * pcircler->radius) + pcircler->duCenter;
    cosOut = (cosOut * pcircler->radius) + pcircler->dvCenter;

    func_001EDAD8(pcircler, sinOut, cosOut);
}
