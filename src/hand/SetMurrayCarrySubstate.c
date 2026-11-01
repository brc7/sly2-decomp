#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef int qword __attribute__((mode(TI)));
typedef struct R { char pad[0x74]; float x74; } R;
typedef struct B { char pad[0x14]; R *pr; char pad2[4]; float x1C; } B;
typedef struct A { B *pb; } A;
typedef struct C { void *x; } C;
typedef struct P {
    char pad0[0x58];
    void *x58, *x5C;
    char pad60[0x190 - 0x60];
    qword x190;
    float x1A0;
    char pad1A4[0x1030 - 0x1A4];
    A a;
    C c;
    char pad1038[0x1320 - 0x1038];
    char x1320[0x1458 - 0x1320];
    int x1458;
    float x145C;
    int pad1460;
    R *x1464;
    char pad1468[0x1510 - 0x1468];
    void *x1510;
    char pad1514[0x151C - 0x1514];
    R *x151C;
} P;
typedef struct { float t,dt; } CLOCK;
extern CLOCK D_002DDED0;
typedef char R_x74_Offset[FIELD_OFFSET(R,x74)==116?1:-1];
typedef char B_pr_Offset[FIELD_OFFSET(B,pr)==20?1:-1];
typedef char B_x1C_Offset[FIELD_OFFSET(B,x1C)==28?1:-1];
typedef char P_x58_Offset[FIELD_OFFSET(P,x58)==88?1:-1];
typedef char P_x5C_Offset[FIELD_OFFSET(P,x5C)==92?1:-1];
typedef char P_x190_Offset[FIELD_OFFSET(P,x190)==400?1:-1];
typedef char P_x1A0_Offset[FIELD_OFFSET(P,x1A0)==416?1:-1];
typedef char P_a_Offset[FIELD_OFFSET(P,a)==4144?1:-1];
typedef char P_c_Offset[FIELD_OFFSET(P,c)==4148?1:-1];
typedef char P_x1320_Offset[FIELD_OFFSET(P,x1320)==4896?1:-1];
typedef char P_x1458_Offset[FIELD_OFFSET(P,x1458)==5208?1:-1];
typedef char P_x145C_Offset[FIELD_OFFSET(P,x145C)==5212?1:-1];
typedef char P_x1464_Offset[FIELD_OFFSET(P,x1464)==5220?1:-1];
typedef char P_x1510_Offset[FIELD_OFFSET(P,x1510)==5392?1:-1];
typedef char P_x151C_Offset[FIELD_OFFSET(P,x151C)==5404?1:-1];
extern qword D_002A2DD0;
void func_002303A0(P *p, void *x, int k);
void func_0020BD68(void *p);
void ReleaseAsega(A *pa);
void ReleaseSma(C *pc);
void ApplyAsegCur(R *pr, P *p, float f, float g, int k, A *pa) __asm__("ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA");
void ReplaceSma(void *pv, P *p, int a, int k, C *pc);

void SetMurrayCarrySubstate(P *p, int k)
{
    R *pr;
    void *pv;
    int n;
    float f,now;

    if (k == p->x1458) return;
    switch (p->x1458) {
    case 0:
        func_002303A0(p, p->x58, 2);
        func_002303A0(p, p->x5C, 2);
        p->x1458 = k;
        break;
    case 1:
    case 2:
        p->x1A0 = p->a.pb->x1C / p->a.pb->pr->x74;
    default:
        p->x1458 = k;
        break;
    }
    __asm__("daddu %0,$%5,$zero\n\tdaddu %1,$%5,$zero\n\tlwc1 %2,%6\n\taddiu %3,$zero,%7\n\tmtc1 $%5,%4":"=&r"(pr),"=&r"(pv),"=f"(now),"=r"(n),"=f"(f):"J"(0),"m"(D_002DDED0.t),"I"(1));
    p->x145C=now;
    if(k==n)goto state1;
    if(k<2){if(k==0)goto state0;goto done;}
    if(k==2)goto state2;
    goto done;
state0:
    func_002303A0(p,p->x58,1);
    func_002303A0(p,p->x5C,1);
    pv=p->x1510;
    p->x190=D_002A2DD0;
    goto done;
state1:
    pr=p->x1464;
    f=p->x1A0*pr->x74;
    func_0020BD68(p->x1320);
    n=9;
    goto done;
state2:
    pr=p->x151C;
    n=9;
    f=p->x1A0*pr->x74;
done:
    ReleaseAsega(&p->a);
    ReleaseSma(&p->c);
    ApplyAsegCur(pr, p, f, 1.0f, n, &p->a);
    ReplaceSma(pv, p, -1, n, &p->c);
}
