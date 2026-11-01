typedef struct LM { float gMin; float gMax; } LM;
extern float D_002BDCD0;
extern LM D_002BDCD8;
struct S { char pad[0x40]; int fl; char pad2[0x10]; float g54; LM lm; float g60; };
void InitLo(struct S *p);
void func_001564E8(struct S *p)
{
    InitLo(p);
    p->g54 = D_002BDCD0;
    p->lm = D_002BDCD8;
    p->g60 = 0.25f;
    p->fl |= 0x40;
}
