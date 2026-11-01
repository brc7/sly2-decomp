struct LM { float a, b; };
struct X { char pad[0xA8]; float *pg; };
struct G { char pad[0x3B0C]; struct X *px; };
extern struct G D_003DD490;
float GLimitLm__FP2LMf(struct LM *plm, float g);
struct S { int pad[5]; struct LM *plm; };
void SetDifficulty(struct S *p, float g)
{
    struct G *pg = &D_003DD490;
    if (pg->px)
        *pg->px->pg = GLimitLm__FP2LMf(p->plm, g);
}
