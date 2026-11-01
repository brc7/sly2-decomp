class O {
public:
    int m_a[3];
    virtual void AddRef();
    virtual void Release();
    virtual void V3(int a, int b, int c, int d, int e);
    virtual void V4(int a, O *p);
};
struct DL;
extern DL D_003E148C;
extern "C" int func_00156870(O *o);
extern "C" void RemoveDlEntry__FP2DLPv(DL *pdl, void *pv);
class CRefGuard {
public:
    O *m_p;
    CRefGuard(O *p) : m_p(p) { p->AddRef(); }
    ~CRefGuard() { m_p->Release(); }
};
/** @sly2-annot addr=0x156780
 * Purpose: Notifies listeners (vt 0xF), unlinks the noise from the active list D_003E148C, sends message 3,
 *   releases
 * Info: proposed name RemoveNoise (T2); TU noise
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
extern "C" void func_00156780(O *o)
{
    if (func_00156870(o))
        return;
    CRefGuard guard(o);
    o->V4(0xF, o);
    if (func_00156870(o))
        return;
    RemoveDlEntry__FP2DLPv(&D_003E148C, o);
    o->V3(3, 0, 0, 0, 0);
    o->Release();
}
