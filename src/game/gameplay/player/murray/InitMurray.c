/* InitMurray: actor defaults init. The int -1 stores come before the float stores in source; gcc schedules them so x17F4 lands right after x220. (Claude agent) */
/* Complete actor defaults and embedded binding initialization. */
struct SUB{float f0;int i;float f1,f2;};
struct BIND{void*owner;char pad4[0x8C];int word90;float f94,f98;};
struct RESET{int word0;};
struct WORDPAIR{unsigned word0,word4;float f8;};
struct CONTEXT{void*owner;int state;char pad8[0xC];float f14,f18,f1C,f20,f24,f28;char pad2C[4];struct WORDPAIR r30;char pad3C[0x84];float fC0;unsigned wordC4,wordC8;char padCC[4];float fD0,fD4,fD8;};
struct S{
 char pad0[0x220];float x220;char pad224[0xDDC];int x1000;char pad1004[0x38];float x103C;char pad1040[8];float x1048;char pad104C[0x4C];float x1098,x109C,x10A0,x10A4;struct SUB sub;
 char pad10B8[0x268];struct BIND bind1320;char pad13BC[0x14];int x13D0;char pad13D4[4];int x13D8;char pad13DC[0x34];float x1410,x1414;char pad1418[0x1C];float x1434;char pad1438[0x14];int x144C,x1450;char pad1454[4];int x1458;char pad145C[4];struct BIND bind1460;char pad14FC[0x44];struct BIND bind1540;char pad15DC[0x54];struct BIND bind1630;char pad16CC[0x128];int x17F4;char pad17F8[0x18];int x1810;char pad1814[0x2C];int x1840;char pad1844[0xC];struct RESET reset1850;char pad1854[0xBC];struct CONTEXT context1910;char pad19EC[0xDC];float x1AC8;
};
extern struct S *D_002F7900;
void InitStepplayer(struct S *p);
void InitGait(struct BIND*a,void*owner,int word90);
void func_0020BD78(struct RESET*a);
void InitThrow(struct CONTEXT*a,void*owner);
static inline void Init(struct SUB *ps, float f, float g) { ps->f2 = g; ps->f1 = f; ps->i = 0; }
/** @sly2-annot addr=0x1B4728
 * Purpose: MURRAY vt7 Init: STEPPLAYER Init, defaults (x220=50, state fields -1, throw range 1500/3500), 4
 *   locomotion blends (0x20B840 x1320/1460/1540/1630), JTTHROW at +0x1910, sets g_pmurray D_002F7900=this
 * Info: proposed name InitMurray (T1); class MURRAY slot7; TU murray
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5b_rest
 * See: docs/engine/player.md#10-source-files, docs/engine/rendering.md#12-source-files,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void InitMurray(struct S *p)
{
    InitStepplayer(p);
    p->x220 = 50.0f;
    p->x13D8 = -1;
    p->x1810 = -1;
    p->x1840 = -1;
    p->x13D0 = -1;
    p->x144C = -1;
    p->x1450 = -1;
    p->x1458 = -1;
    p->x17F4 = -1;
    p->x1410 = 1500.0f;
    p->x1414 = 3500.0f;
    p->x103C = 60.0f;
    p->x1048 = 125.0f;
    InitGait(&p->bind1320, p, 0x15);
    InitGait(&p->bind1460, p, 0x15);
    InitGait(&p->bind1630, p, 0x15);
    InitGait(&p->bind1540, p, 0x15);
    func_0020BD78(&p->reset1850);
    {
    register float a __asm__("$f6") = -60.0f;
    register float b __asm__("$f4") = -100.0f;
    register float c __asm__("$f0") = 100.0f;
    register float d __asm__("$f1") = -10.0f;
    register float e __asm__("$f2") = 250.0f;
    register float f __asm__("$f5") = 1.0f;
    register float g __asm__("$f3") = 4500.0f;
    p->x1AC8=a; p->x1434=a; p->x1098=b; p->x109C=c; p->x10A0=50.0f; p->x10A4=d; p->sub.f0=e;
    Init(&p->sub, f, g);
    }
    InitThrow(&p->context1910, p);
    p->x1000 = 4;
    D_002F7900 = p;
}

typedef char offset_0_x220[(unsigned)&((struct S*)0)->x220==544?1:-1];
typedef char offset_0_x1000[(unsigned)&((struct S*)0)->x1000==4096?1:-1];
typedef char offset_0_x103C[(unsigned)&((struct S*)0)->x103C==4156?1:-1];
typedef char offset_0_x1048[(unsigned)&((struct S*)0)->x1048==4168?1:-1];
typedef char offset_0_x1098[(unsigned)&((struct S*)0)->x1098==4248?1:-1];
typedef char offset_0_x109C[(unsigned)&((struct S*)0)->x109C==4252?1:-1];
typedef char offset_0_x10A0[(unsigned)&((struct S*)0)->x10A0==4256?1:-1];
typedef char offset_0_x10A4[(unsigned)&((struct S*)0)->x10A4==4260?1:-1];
typedef char offset_0_x13D0[(unsigned)&((struct S*)0)->x13D0==5072?1:-1];
typedef char offset_0_x13D8[(unsigned)&((struct S*)0)->x13D8==5080?1:-1];
typedef char offset_0_x1410[(unsigned)&((struct S*)0)->x1410==5136?1:-1];
typedef char offset_0_x1414[(unsigned)&((struct S*)0)->x1414==5140?1:-1];
typedef char offset_0_x1434[(unsigned)&((struct S*)0)->x1434==5172?1:-1];
typedef char offset_0_x144C[(unsigned)&((struct S*)0)->x144C==5196?1:-1];
typedef char offset_0_x1450[(unsigned)&((struct S*)0)->x1450==5200?1:-1];
typedef char offset_0_x1458[(unsigned)&((struct S*)0)->x1458==5208?1:-1];
typedef char offset_0_x17F4[(unsigned)&((struct S*)0)->x17F4==6132?1:-1];
typedef char offset_0_x1810[(unsigned)&((struct S*)0)->x1810==6160?1:-1];
typedef char offset_0_x1840[(unsigned)&((struct S*)0)->x1840==6208?1:-1];
typedef char offset_0_x1AC8[(unsigned)&((struct S*)0)->x1AC8==6856?1:-1];
typedef char offset_0_sub[(unsigned)&((struct S*)0)->sub==4264?1:-1];
typedef char offset_0_bind1320[(unsigned)&((struct S*)0)->bind1320==4896?1:-1];
typedef char offset_0_bind1460[(unsigned)&((struct S*)0)->bind1460==5216?1:-1];
typedef char offset_0_bind1540[(unsigned)&((struct S*)0)->bind1540==5440?1:-1];
typedef char offset_0_bind1630[(unsigned)&((struct S*)0)->bind1630==5680?1:-1];
typedef char offset_0_reset1850[(unsigned)&((struct S*)0)->reset1850==6224?1:-1];
typedef char offset_0_context1910[(unsigned)&((struct S*)0)->context1910==6416?1:-1];
typedef char offset_1_f0[(unsigned)&((struct SUB*)0)->f0==0?1:-1];
typedef char offset_1_i[(unsigned)&((struct SUB*)0)->i==4?1:-1];
typedef char offset_1_f1[(unsigned)&((struct SUB*)0)->f1==8?1:-1];
typedef char offset_1_f2[(unsigned)&((struct SUB*)0)->f2==12?1:-1];
typedef char offset_2_owner[(unsigned)&((struct BIND*)0)->owner==0?1:-1];
typedef char offset_2_word90[(unsigned)&((struct BIND*)0)->word90==144?1:-1];
typedef char offset_2_f94[(unsigned)&((struct BIND*)0)->f94==148?1:-1];
typedef char offset_2_f98[(unsigned)&((struct BIND*)0)->f98==152?1:-1];
typedef char offset_3_word0[(unsigned)&((struct RESET*)0)->word0==0?1:-1];
typedef char offset_4_word0[(unsigned)&((struct WORDPAIR*)0)->word0==0?1:-1];
typedef char offset_4_word4[(unsigned)&((struct WORDPAIR*)0)->word4==4?1:-1];
typedef char offset_4_f8[(unsigned)&((struct WORDPAIR*)0)->f8==8?1:-1];
typedef char offset_5_owner[(unsigned)&((struct CONTEXT*)0)->owner==0?1:-1];
typedef char offset_5_state[(unsigned)&((struct CONTEXT*)0)->state==4?1:-1];
typedef char offset_5_f14[(unsigned)&((struct CONTEXT*)0)->f14==20?1:-1];
typedef char offset_5_f18[(unsigned)&((struct CONTEXT*)0)->f18==24?1:-1];
typedef char offset_5_f1C[(unsigned)&((struct CONTEXT*)0)->f1C==28?1:-1];
typedef char offset_5_f20[(unsigned)&((struct CONTEXT*)0)->f20==32?1:-1];
typedef char offset_5_f24[(unsigned)&((struct CONTEXT*)0)->f24==36?1:-1];
typedef char offset_5_f28[(unsigned)&((struct CONTEXT*)0)->f28==40?1:-1];
typedef char offset_5_r30[(unsigned)&((struct CONTEXT*)0)->r30==48?1:-1];
typedef char offset_5_fC0[(unsigned)&((struct CONTEXT*)0)->fC0==192?1:-1];
typedef char offset_5_wordC4[(unsigned)&((struct CONTEXT*)0)->wordC4==196?1:-1];
typedef char offset_5_wordC8[(unsigned)&((struct CONTEXT*)0)->wordC8==200?1:-1];
typedef char offset_5_fD0[(unsigned)&((struct CONTEXT*)0)->fD0==208?1:-1];
typedef char offset_5_fD4[(unsigned)&((struct CONTEXT*)0)->fD4==212?1:-1];
typedef char offset_5_fD8[(unsigned)&((struct CONTEXT*)0)->fD8==216?1:-1];
typedef char size_0[sizeof(struct S)==6860?1:-1];
typedef char size_1[sizeof(struct SUB)==16?1:-1];
typedef char size_2[sizeof(struct BIND)==156?1:-1];
typedef char size_3[sizeof(struct RESET)==4?1:-1];
typedef char size_4[sizeof(struct WORDPAIR)==12?1:-1];
typedef char size_5[sizeof(struct CONTEXT)==220?1:-1];
typedef char size_6[sizeof(int)==4?1:-1];
typedef char size_7[sizeof(float)==4?1:-1];
typedef char size_8[sizeof(unsigned)==4?1:-1];
typedef char size_9[sizeof(void*)==4?1:-1];
typedef char align_0[__alignof__(struct S)==4?1:-1];
typedef char align_1[__alignof__(struct SUB)==4?1:-1];
typedef char align_2[__alignof__(struct BIND)==4?1:-1];
typedef char align_3[__alignof__(struct RESET)==4?1:-1];
typedef char align_4[__alignof__(struct WORDPAIR)==4?1:-1];
typedef char align_5[__alignof__(struct CONTEXT)==4?1:-1];
