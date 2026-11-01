typedef struct VECTOR { float x, y, z; } VECTOR;
struct S { int n; char pad[0xC]; VECTOR v; char pad2[0x8C]; int a; };
void InitReconSel(struct S *p, int a)
{
    VECTOR *pv = &p->v;
    float y;
p->a = a;
pv->x = 0;
pv->y = 0;
pv->z = 0;
    y = pv->y;
    pv->x = pv->x * 320.0f + 320.0f;
    pv->y = 246.40000915527344f;
    pv->z = y;
    p->n = -1;
}
