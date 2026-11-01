typedef struct { float x, y, dx, dy; char pad[0x10]; } TXT;
typedef struct { char ab[0xA0]; } FONT;
typedef struct { int n; int a; int b; } SLOT;
typedef struct { char pad[0x1180]; SLOT aslot[3]; } Q;
typedef struct { int n; char pad[0x10]; float x; float y; char pad1c[4]; } ENT;
typedef struct {
    char pad[0x24];
    void *x24;
    char pad28[0x4C - 0x28];
    int c;
    ENT *ae;
} OBJ;
extern struct G D_003DD490;
extern float D_002C3264;
extern int D_002C3268[];
int func_002132D0(void *);
Q *PpoHero(struct G *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void func_00258438(TXT *, float, float);
void func_00258448(TXT *, float, float);
void func_00258458(TXT *, unsigned int *);
void func_00258470(TXT *, int);
void func_00258478(TXT *, int);
FONT *func_00215AC0(FONT *, int, float);
void func_002163B0(FONT *, TXT *, int);
static inline float Zero(void){float z;__asm__ volatile("mtc1 $zero,%0":"=f"(z));return z;}
void func_001DAC58(OBJ *p)
{
    Q *q;
    int i;
    if (p->x24 == 0)
        return;
    if (!func_002132D0(p->x24))
        return;
    q = PpoHero(&D_003DD490);
    if (q == 0)
        return;
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x34))
        return;
    for (i = 0; i < p->c; i++) {
        ENT *pe = &p->ae[i];
        int j;
        for (j = 0; j < 3; j++) {
            if (pe->n == q->aslot[j].n) {
                TXT txt;
                unsigned int rgba[2];
                FONT font;
                {float z=Zero();func_00258438(&txt,z,z);}
                func_00258448(&txt, 640.0f, 448.0f * 1.1f);
                rgba[0] = 0xFF808080;
                func_00258458(&txt, rgba);
                func_00258470(&txt, 0);
                func_00258478(&txt, 0);
                func_00215AC0(&font, D_002C3268[j], D_002C3264);
                {float x=pe->x;float y=pe->y;txt.y=y;txt.x=x;}
                txt.dx = 32.0f;
                txt.dy = 32.0f;
                func_002163B0(&font, &txt, 0x13);
                break;
            }
        }
    }
}
