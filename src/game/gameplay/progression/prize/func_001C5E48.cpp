typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t, dt; };
extern "C" {
extern struct CLOCK D_002DDED0;
extern float D_002C0D50;
}
class LOB {
public:
    int a[3];
    virtual void V1(); virtual void V2(); virtual void V3(); virtual void V4(); virtual void V5();
    virtual void V6(); virtual void V7(); virtual void V8(); virtual void V9(); virtual void V10();
    virtual void V11(); virtual void V12(); virtual void V13(); virtual void V14(); virtual void V15();
    virtual void V16(); virtual void V17(); virtual void V18(); virtual void V19(); virtual void V20();
    virtual void V21(); virtual void V22(); virtual void V23(); virtual void V24(); virtual void V25();
    virtual void V26(); virtual void V27(); virtual void V28(); virtual void V29(); virtual void V30();
    virtual void V31(); virtual void V32(); virtual void V33(); virtual void V34(); virtual void V35();
    virtual void V36(); virtual void V37(); virtual void V38(); virtual void V39(); virtual void V40();
    virtual void V41(); virtual void V42(); virtual void V43(); virtual void V44(); virtual void V45();
    virtual void V46(); virtual void V47(); virtual void V48(); virtual void V49(); virtual void V50();
    virtual void V51(); virtual void V52(); virtual void V53(); virtual void V54(); virtual void V55();
    virtual void V56(); virtual void V57(); virtual void V58(); virtual void V59(); virtual void V60();
    virtual void V61(); virtual void V62(); virtual void V63(); virtual void V64(); virtual void V65();
    virtual void V66(); virtual void V67(); virtual void V68(); virtual void V69(); virtual void V70();
    virtual void V71(); virtual void V72(); virtual void V73(); virtual void V74(); virtual void V75();
    virtual void SetState(int n);
};
class P : public LOB {
public:
    char pad[0x150 - 0x10];
    int state;
    float t154;
    char pad2[0x180 - 0x158];
    qword pos;
    char pad3[0x1C4 - 0x190];
    int f1c4;
};
extern "C" {
void UpdateSo(P *p);
void func_001C6308(P *p);
void func_001C55E0(P *p);
void func_001C5D18(P *p);
void func_001C5E48(P *p);
}
static inline float LenSq(qword v)
{
    float r;
    qword t, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(t), "=&j"(one) : "j"(v));
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(t));
    return r;
}
/** @sly2-annot addr=0x1C5E48
 * Purpose: DPRIZE vt16 Update: base update, sparkle (0x1C6308), bounce, collect
 * Info: proposed name UpdateDprize (T1); class DPRIZE slot16; TU prize
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C5E48(P *p)
{
    UpdateSo(p);
    func_001C6308(p);
    switch (p->state) {
    case 0:
        if (LenSq(p->pos) > 10000.0f)
            func_001C55E0(p);
    case 5:
        func_001C5D18(p);
        break;
    case 3:
        if (*(float *)((char *)&p->pos + 8) < 0.0f)
            p->SetState(4);
        break;
    case 4:
        if (p->f1c4 && *(float *)((char *)&p->pos + 8) < 25.0f) {
            p->f1c4 = 0;
            p->SetState(0);
        } else if (D_002C0D50 < D_002DDED0.t - p->t154) {
            p->SetState(2);
        } else {
            func_001C55E0(p);
            func_001C5D18(p);
        }
        break;
    case 1:
    case 2:
    case 6:
        break;
    }
}
