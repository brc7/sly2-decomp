/* Event controller. Below/Equal are true when the selector is NULL (old draft had them false). */
struct SELECTOR;
struct Q { char pad[0x58]; int x58; };
struct OO { char pad[0x1A8]; float x1a8; char pad2[0x10E4-0x1AC]; int x10e4; int pad3; int x10ec; };
struct O { struct OO *x0; char pad[0x20-4]; int x20; char pad2[0x3C-0x24]; float x3c; };
struct P { struct O *x0; int x4; struct Q *x8; int xc; struct Q *x10; SELECTOR*x14; };
extern "C" int D_002BF920;
extern "C" void UpdateReconSel(SELECTOR*p);
extern "C" void func_0019D660(struct P *p, int n);
extern "C" void func_00213B00(struct Q *q, int n);
extern "C" void func_00213928(struct Q *q, int n);
extern "C" void func_00212E40(struct Q *q, int n, float g);
static inline void Below(int&value,Q*q){value=0;if(!q||q->x58<2)value=1;}
static inline void Equal(int&value,Q*q){value=0;if(!q||q->x58==2)value=1;}
extern "C" void func_0019DA78(struct P *p)
{

    if (p->x14)
        UpdateReconSel(p->x14);
    switch (p->x4) {
    case -1:
        return;
    case 2:{
        int a,b;
        Below(a,p->x8);
        b = 1;
        if (p->x0->x20 == 1) {
            Below(b,p->x10);
        }
        if (a && b)
            func_0019D660(p, -1);
        return;
    }
    case 0:{
        int a,b;
        Equal(a,p->x8);
        D_002BF920 = -1;
        b = 1;
        if (p->x0->x20 == 1) {
            Equal(b,p->x10);
        }
        if (a && b)
            func_0019D660(p, 1);
        break;
    }
    }
    if (!p->x8)
        return;
    if (p->x4) {
        int previous=p->x0->x0->x10ec;
        if(previous==4){
            if(D_002BF920!=previous)func_00213B00(p->x8,0x353);
            if(p->x0->x0->x10ec==previous)goto store;
        }
        if(D_002BF920==4)func_00213928(p->x8,0x353);
store:
        D_002BF920=p->x0->x0->x10e4;
    }
    func_00212E40(p->x8, 0x352, (p->x0->x0->x1a8 + 3.14159265f) * 0.15915494f);
    func_00212E40(p->x8, 0x351, p->x0->x3c);
}

typedef char offset_Q_x58[(unsigned)&((Q*)0)->x58==88?1:-1];
typedef char offset_OO_x1a8[(unsigned)&((OO*)0)->x1a8==424?1:-1];
typedef char offset_OO_x10e4[(unsigned)&((OO*)0)->x10e4==4324?1:-1];
typedef char offset_OO_x10ec[(unsigned)&((OO*)0)->x10ec==4332?1:-1];
typedef char offset_O_x0[(unsigned)&((O*)0)->x0==0?1:-1];
typedef char offset_O_x20[(unsigned)&((O*)0)->x20==32?1:-1];
typedef char offset_O_x3c[(unsigned)&((O*)0)->x3c==60?1:-1];
typedef char offset_P_x0[(unsigned)&((P*)0)->x0==0?1:-1];
typedef char offset_P_x4[(unsigned)&((P*)0)->x4==4?1:-1];
typedef char offset_P_x8[(unsigned)&((P*)0)->x8==8?1:-1];
typedef char offset_P_x10[(unsigned)&((P*)0)->x10==16?1:-1];
typedef char offset_P_x14[(unsigned)&((P*)0)->x14==20?1:-1];
typedef char size_0[sizeof(Q)==92?1:-1];
typedef char size_1[sizeof(OO)==4336?1:-1];
typedef char size_2[sizeof(O)==64?1:-1];
typedef char size_3[sizeof(P)==24?1:-1];
typedef char size_4[sizeof(int)==4?1:-1];
typedef char size_5[sizeof(float)==4?1:-1];
typedef char size_6[sizeof(void*)==4?1:-1];
typedef char align_0[__alignof__(Q)==4?1:-1];
typedef char align_1[__alignof__(OO)==4?1:-1];
typedef char align_2[__alignof__(O)==4?1:-1];
typedef char align_3[__alignof__(P)==4?1:-1];
