typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x98]; short k; char pad2[0xA0 - 0x9A]; int nA0; char pad3[0xB0 - 0xA4]; qword q; };
struct P { char pad[0x5C]; struct O *po; };
struct Q { char pad[0x60]; int n60; };
int func_00230310(int);
void func_002303A0(void *, struct O *, int);
qword func_0022FCC8(int, int, qword);
void func_00230598(void *, struct O *);
void func_001F4BF8(void *);
void func_001F5078(void *);
void func_001F8668(void *, int);
/** @sly2-annot addr=0x1F4930
 * Purpose: Same for rotation XF
 * Info: proposed name SetSoRotXfVelocityLocal (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4930(struct P *p, qword v)
{
    struct O *po;
    if (!func_00230310(p->po->k)) func_002303A0(p, p->po, 1);
    po = p->po;
    po->q = func_0022FCC8(((struct Q *)p)->n60, po->nA0, v);
    func_00230598(p, p->po);
    func_001F4BF8(p);
    func_001F5078(p);
    func_001F8668(p, 0);
}
