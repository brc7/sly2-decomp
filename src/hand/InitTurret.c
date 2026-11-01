struct V2 { float x, y; };
struct S { char pad[0x19C]; float a, b, c, d; struct V2 e; float pad2[2]; float f; int pad3[3]; int g; void *h; int i; };
extern struct V2 D_002C4400;
void InitPo(struct S *p);
void InitTurret(struct S *p)
{
    InitPo(p);
    p->e = D_002C4400;
    p->g = 1; /*P*/
    p->f = p->e.y; /*P*/
    p->c = -1.5f; /*P*/
    p->d = 1.5f; /*P*/
    p->a = -0.7f; /*P*/
    p->b = 0.7f; /*P*/
    p->i = 1; /*P*/
    p->h = p; /*P*/
}
